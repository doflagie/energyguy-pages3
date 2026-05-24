// ============================================================
// esp32_rotary_loop.ino — TM-ADF-005 Rev A
// Motorized Rotary Loop RDF Controller
// ============================================================

#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>

#include "config.h"
#include "loop_rdf.h"

LoopRDF          loop_rdf(GPIO_MOTOR_STEP, GPIO_MOTOR_DIR, GPIO_MOTOR_EN,
                           GPIO_POS_ADC, GPIO_SIG_ADC);
WebServer        httpServer(HTTP_PORT);
BluetoothSerial  SerialBT;
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Preferences      prefs;

static LoopRDFResult g_result;
static uint8_t       g_band_idx    = 3;  // default 40M
static bool          g_result_new  = false;
static uint32_t      g_oled_ms     = 0;
static uint32_t      g_bt_ms       = 0;
static char          g_mode_str[12] = "IDLE";

// ─── Band switching ─────────────────────────────────────────
void selectBand(uint8_t idx) {
    g_band_idx = idx;
    uint8_t bits = LOOP_BANDS[idx].relay_bits;
    digitalWrite(GPIO_BAND_R0, bits & 0x01);
    digitalWrite(GPIO_BAND_R1, (bits >> 1) & 0x01);
    digitalWrite(GPIO_BAND_R2, (bits >> 2) & 0x01);
    loop_rdf.setTuneVoltage(LOOP_BANDS[idx].tune_dac_v);
    prefs.putUChar("band", idx);
}

// ─── OLED ───────────────────────────────────────────────────
void updateOLED() {
    oled.clearDisplay();
    oled.setTextSize(1); oled.setTextColor(SSD1306_WHITE);

    oled.setCursor(0, 0);
    oled.printf("LOOP  %s  %s", LOOP_BANDS[g_band_idx].name, g_mode_str);

    oled.setCursor(0, 16);
    oled.printf("POS: %06.2f\xb0", loop_rdf.getPosition());

    oled.setCursor(0, 32);
    if (g_result.confidence > 0.1f) {
        const char* dirs[] = {"N","NE","E","SE","S","SW","W","NW"};
        int di = (int)((g_result.bearing_mag + 22.5f) / 45.0f) % 8;
        oled.printf("NULL:%05.1f\xb0 %s", g_result.bearing_mag, dirs[di]);
    } else {
        oled.print("NULL: --- SCANNING");
    }

    oled.setCursor(0, 48);
    oled.printf("D:%5.1fdB C:%3.0f%%",
                g_result.null_depth_db, g_result.confidence * 100.0f);

    oled.display();
}

// ─── BT telemetry ───────────────────────────────────────────
void sendBT() {
    SerialBT.printf("POS:%.1f,NULL:%.1f,DEPTH:%.1f,CNF:%.2f,BAND:%s,MODE:%s\n",
        loop_rdf.getPosition(),
        g_result.bearing_mag, g_result.null_depth_db,
        g_result.confidence, LOOP_BANDS[g_band_idx].name, g_mode_str);
}

// ─── BT command parser ───────────────────────────────────────
static char bt_buf[64]; static uint8_t bt_pos = 0;
void parseBT() {
    while (SerialBT.available()) {
        char c = SerialBT.read();
        if (c == '\n') {
            bt_buf[bt_pos] = '\0';
            if (bt_pos > 0) {
                if (strcmp(bt_buf, "SCAN") == 0) {
                    loop_rdf.startFullScan();
                    strncpy(g_mode_str, "SCANNING", sizeof(g_mode_str));
                    SerialBT.println("OK SCAN STARTED");
                } else if (strcmp(bt_buf, "STOP") == 0) {
                    loop_rdf.stop();
                    strncpy(g_mode_str, "IDLE", sizeof(g_mode_str));
                    SerialBT.println("OK STOPPED");
                } else if (strcmp(bt_buf, "HOME") == 0) {
                    loop_rdf.home();
                    strncpy(g_mode_str, "HOMING", sizeof(g_mode_str));
                    SerialBT.println("OK HOMING");
                } else if (strncmp(bt_buf, "MOVE ", 5) == 0) {
                    float deg = atof(bt_buf + 5);
                    loop_rdf.moveTo(deg);
                    SerialBT.printf("OK MOVE %.1f\n", deg);
                } else if (strncmp(bt_buf, "BAND ", 5) == 0) {
                    int n = atoi(bt_buf + 5);
                    if (n >= 0 && n < NUM_LOOP_BANDS) {
                        selectBand(n);
                        SerialBT.printf("OK BAND %s\n", LOOP_BANDS[n].name);
                    }
                } else if (strcmp(bt_buf, "SENSE ON") == 0) {
                    loop_rdf.enableSense(true);
                    SerialBT.println("OK SENSE ON");
                } else if (strcmp(bt_buf, "SENSE OFF") == 0) {
                    loop_rdf.enableSense(false);
                    SerialBT.println("OK SENSE OFF");
                } else if (strcmp(bt_buf, "TUNE") == 0) {
                    loop_rdf.autoTune();
                    SerialBT.println("OK TUNED");
                } else if (strcmp(bt_buf, "STATUS") == 0) {
                    sendBT();
                } else {
                    SerialBT.printf("ERR: %s\n", bt_buf);
                }
            }
            bt_pos = 0;
        } else if (bt_pos < 63) {
            bt_buf[bt_pos++] = c;
        }
    }
}

// ─── Web handlers ───────────────────────────────────────────
static const char LOOP_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Loop RDF</title>
<style>
body{background:#111;color:#0f0;font-family:monospace;text-align:center}
svg{display:block;margin:0 auto}
.btn{background:#222;color:#0f0;border:1px solid #0f0;padding:6px 12px;margin:3px;cursor:pointer}
.btn:hover{background:#0f0;color:#000}
#status{margin:8px;font-size:0.9em}
</style></head><body>
<h2>Loop RDF</h2>
<svg width="260" height="260" viewBox="0 0 260 260">
  <circle cx="130" cy="130" r="120" fill="none" stroke="#0f0" stroke-width="2"/>
  <!-- Figure-8 pattern (loop only) -->
  <ellipse id="lobe-n" cx="130" cy="90" rx="35" ry="50" fill="rgba(0,255,0,0.08)" stroke="#040" stroke-width="1"/>
  <ellipse id="lobe-s" cx="130" cy="170" rx="35" ry="50" fill="rgba(0,255,0,0.08)" stroke="#040" stroke-width="1"/>
  <text x="130" y="18" fill="#0f0" text-anchor="middle" font-size="13">N</text>
  <text x="248" y="134" fill="#0f0" text-anchor="middle" font-size="13">E</text>
  <text x="130" y="252" fill="#0f0" text-anchor="middle" font-size="13">S</text>
  <text x="12" y="134" fill="#0f0" text-anchor="middle" font-size="13">W</text>
  <!-- Loop position indicator -->
  <line id="loop-line" x1="130" y1="20" x2="130" y2="240"
        stroke="#0a0" stroke-width="2" stroke-dasharray="5,5"
        transform="rotate(0,130,130)"/>
  <!-- Null bearing needle (red) -->
  <line id="null-needle" x1="130" y1="130" x2="130" y2="20"
        stroke="#f00" stroke-width="2"
        transform="rotate(0,130,130)"/>
  <circle cx="130" cy="130" r="5" fill="#f00"/>
  <text id="brg-lbl" x="130" y="148" fill="#ff0" text-anchor="middle" font-size="14">---</text>
</svg>
<div id="status">Depth: --- | Conf: --- | Mode: ---</div>
<div style="margin:6px">
  <button class="btn" onclick="fetch('/api/scan',{method:'POST'})">SCAN</button>
  <button class="btn" onclick="fetch('/api/stop',{method:'POST'})">STOP</button>
  <button class="btn" onclick="fetch('/api/home',{method:'POST'})">HOME</button>
  <button class="btn" onclick="fetch('/api/tune',{method:'POST'})">TUNE</button>
</div>
<div id="band-grid" style="display:flex;flex-wrap:wrap;justify-content:center;max-width:320px;margin:0 auto"></div>
<script>
const bands=['160M','80M','60M','40M','30M','20M','17M','15M'];
const bg=document.getElementById('band-grid');
bands.forEach((b,i)=>{
  const bt=document.createElement('button');
  bt.className='btn'; bt.textContent=b;
  bt.onclick=()=>fetch('/api/band?n='+i,{method:'POST'});
  bg.appendChild(bt);
});
function refresh(){
  fetch('/api/status').then(r=>r.json()).then(d=>{
    document.getElementById('loop-line').setAttribute('transform','rotate('+d.pos+',130,130)');
    if(d.null_deg>=0){
      document.getElementById('null-needle').setAttribute('transform','rotate('+d.null_deg+',130,130)');
      document.getElementById('brg-lbl').textContent=d.null_deg.toFixed(1)+'°';
    }
    document.getElementById('status').textContent=
      'Depth:'+d.depth.toFixed(1)+'dB Conf:'+Math.round(d.conf*100)+'% Mode:'+d.mode;
  }).catch(()=>{});
}
setInterval(refresh,500);
</script></body></html>
)rawhtml";

void handleRoot()  { httpServer.send_P(200, "text/html", LOOP_HTML); }
void handleStatus() {
    char buf[256];
    snprintf(buf, sizeof(buf),
        "{\"pos\":%.1f,\"null_deg\":%.1f,\"depth\":%.1f,\"conf\":%.2f,\"mode\":\"%s\",\"band\":\"%s\"}",
        loop_rdf.getPosition(), g_result.bearing_mag, g_result.null_depth_db,
        g_result.confidence, g_mode_str, LOOP_BANDS[g_band_idx].name);
    httpServer.send(200, "application/json", buf);
}
void handleScan() { loop_rdf.startFullScan(); strncpy(g_mode_str,"SCAN",12); httpServer.send(200,"text/plain","OK"); }
void handleStop() { loop_rdf.stop();          strncpy(g_mode_str,"IDLE",12); httpServer.send(200,"text/plain","OK"); }
void handleHome() { loop_rdf.home();          strncpy(g_mode_str,"HOME",12); httpServer.send(200,"text/plain","OK"); }
void handleTune() { loop_rdf.autoTune();                                      httpServer.send(200,"text/plain","OK"); }
void handleBand() {
    if (httpServer.hasArg("n")) {
        int n = httpServer.arg("n").toInt();
        if (n >= 0 && n < NUM_LOOP_BANDS) selectBand(n);
    }
    httpServer.send(200,"text/plain","OK");
}

// ─── setup() ────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // GPIO
    pinMode(GPIO_BAND_R0, OUTPUT); digitalWrite(GPIO_BAND_R0, LOW);
    pinMode(GPIO_BAND_R1, OUTPUT); digitalWrite(GPIO_BAND_R1, LOW);
    pinMode(GPIO_BAND_R2, OUTPUT); digitalWrite(GPIO_BAND_R2, LOW);
    pinMode(GPIO_BT_LED,  OUTPUT); digitalWrite(GPIO_BT_LED, LOW);
    analogSetAttenuation(ADC_ATTEN_DB_11);
    analogSetWidth(12);

    Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL);
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.clearDisplay();
    oled.setTextSize(2); oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(5, 10); oled.println("LOOP RDF");
    oled.setTextSize(1); oled.setCursor(5, 40);
    oled.println("TM-ADF-005 Rev A");
    oled.display(); delay(1200);

    prefs.begin("loop_rdf", false);
    g_band_idx = prefs.getUChar("band", 3);

    loop_rdf.begin();
    loop_rdf.home();
    strncpy(g_mode_str, "HOMING", sizeof(g_mode_str));

    selectBand(g_band_idx);

    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID);

    httpServer.on("/",          HTTP_GET,  handleRoot);
    httpServer.on("/api/status",HTTP_GET,  handleStatus);
    httpServer.on("/api/scan",  HTTP_POST, handleScan);
    httpServer.on("/api/stop",  HTTP_POST, handleStop);
    httpServer.on("/api/home",  HTTP_POST, handleHome);
    httpServer.on("/api/tune",  HTTP_POST, handleTune);
    httpServer.on("/api/band",  HTTP_POST, handleBand);
    httpServer.begin();

    SerialBT.begin(BT_NAME);
    digitalWrite(GPIO_BT_LED, HIGH);

    Serial.println("Loop RDF ready.");
}

// ─── loop() ─────────────────────────────────────────────────
void loop() {
    loop_rdf.update();

    if (loop_rdf.isResultReady()) {
        g_result = loop_rdf.getResult();
        g_result_new = true;
        loop_rdf.clearResult();
        strncpy(g_mode_str, "IDLE", sizeof(g_mode_str));
    }

    uint32_t now = millis();

    if (now - g_oled_ms >= 250) {
        g_oled_ms = now;
        updateOLED();
    }

    if (now - g_bt_ms >= 1000) {
        g_bt_ms = now;
        if (SerialBT.hasClient()) sendBT();
    }

    parseBT();
    httpServer.handleClient();
}
