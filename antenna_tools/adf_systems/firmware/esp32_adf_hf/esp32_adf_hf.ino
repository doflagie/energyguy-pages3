// ============================================================
// esp32_adf_hf.ino — TM-ADF-001 Rev A
// Watson-Watt HF Automatic Direction Finder
// ESP32-WROOM-32, Arduino framework
//
// Requires libraries:
//   Adafruit SSD1306 + GFX
//   BluetoothSerial (ESP32 built-in)
//   WiFi + WebServer (ESP32 built-in)
//   Wire (built-in)
// ============================================================

#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <nvs_flash.h>
#include <Preferences.h>

#include "config.h"
#include "watson_watt.h"
#include "bearing.h"

// ─── Objects ────────────────────────────────────────────────
WatsonWatt        ww;
BearingDisplay    bd;
WebServer         httpServer(HTTP_PORT);
BluetoothSerial   SerialBT;
HardwareSerial    gpsSerial(GPS_UART_NUM);
Adafruit_SSD1306  oled(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
Preferences       prefs;

// ─── State ──────────────────────────────────────────────────
static WatsonWattResult g_result;
static uint8_t          g_band_idx   = 3;   // default 40M
static bool             g_bt_connected = false;
static uint32_t         g_last_bearing_ms = 0;
static uint32_t         g_last_oled_ms    = 0;
static uint32_t         g_last_bt_ms      = 0;

// ADC sample buffers
static int16_t ns_buf[ADC_BLOCK_SIZE];
static int16_t ew_buf[ADC_BLOCK_SIZE];
static int16_t sense_buf[ADC_BLOCK_SIZE];

// ─── RF Switch helpers ───────────────────────────────────────
void rf_switch_select(uint8_t port) {
    digitalWrite(GPIO_RF_SW_A, port & 0x01);
    digitalWrite(GPIO_RF_SW_B, (port >> 1) & 0x01);
    delayMicroseconds(50);
}

// ─── ADC sampling ───────────────────────────────────────────
// Collect ADC_BLOCK_SIZE samples from the selected ADC pin
// Returns signed values centered around midpoint (2048)
void collectSamples(uint8_t adc_pin, int16_t* buf, size_t count) {
    for (size_t i = 0; i < count; i++) {
        buf[i] = (int16_t)analogRead(adc_pin) - 2048;
        delayMicroseconds(100);  // ~10 kHz
    }
}

void measureBearing() {
    // Switch to N-S pair, collect samples
    rf_switch_select(RF_SW_NS);
    delayMicroseconds(500);  // settle time
    collectSamples(GPIO_NS_ADC, ns_buf, ADC_BLOCK_SIZE);

    // Switch to E-W pair
    rf_switch_select(RF_SW_EW);
    delayMicroseconds(500);
    collectSamples(GPIO_EW_ADC, ew_buf, ADC_BLOCK_SIZE);

    // Switch to sense
    rf_switch_select(RF_SW_SENSE);
    delayMicroseconds(500);
    collectSamples(GPIO_SENSE_ADC, sense_buf, ADC_BLOCK_SIZE);

    // Disable switch (optional power save)
    rf_switch_select(RF_SW_OFF);

    // Compute bearing
    g_result = ww.compute(ns_buf, ew_buf, sense_buf, ADC_BLOCK_SIZE);
}

// ─── OLED Display ───────────────────────────────────────────
void updateOLED() {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);

    // Line 1: mode + band
    oled.setCursor(0, 0);
    oled.printf("ADF HF  %s", HF_BANDS[g_band_idx].name);

    // Line 2: bearing
    oled.setCursor(0, 16);
    if (g_result.bearing_mag >= 0.0f) {
        char bstr[16];
        bd.formatBearing(g_result.bearing_mag, bstr, sizeof(bstr));
        oled.printf("BRG: %s", bstr);
    } else {
        oled.print("BRG: --- (no sig)");
    }

    // Line 3: SNR + confidence
    oled.setCursor(0, 32);
    if (g_result.bearing_mag >= 0.0f) {
        oled.printf("SNR:%4.0fdB C:%3.0f%%",
                    g_result.snr_db, g_result.confidence * 100.0f);
    } else {
        oled.print("SNR: --- C:  0%");
    }

    // Line 4: GPS + compass status
    oled.setCursor(0, 48);
    bool gps_ok  = bd.hasGPSFix();
    bool cmps_ok = bd.getCompass().calibrated;
    oled.printf("GPS:%s CMPS:%s",
                gps_ok  ? "OK " : "---",
                cmps_ok ? "OK" : "--");

    oled.display();
}

// ─── Bluetooth output ────────────────────────────────────────
void sendBTStatus() {
    GPSFix gps = bd.getGPSFix();
    float  true_brg = (g_result.bearing_mag >= 0.0f && gps.valid)
                      ? bd.magneticToTrue(g_result.bearing_mag, gps.lat, gps.lon)
                      : g_result.bearing_mag;
    SerialBT.printf(
        "BRG:%.1f,TRUE:%.1f,SNR:%.1f,CNF:%.2f,BAND:%s,LAT:%.5f,LON:%.5f,SAT:%d\n",
        g_result.bearing_mag >= 0.0f ? g_result.bearing_mag : 0.0f,
        true_brg >= 0.0f ? true_brg : 0.0f,
        g_result.snr_db,
        g_result.confidence,
        HF_BANDS[g_band_idx].name,
        gps.lat, gps.lon, gps.satellites
    );
}

// ─── BT Command Parser ───────────────────────────────────────
static char bt_cmd_buf[64];
static uint8_t bt_cmd_pos = 0;

void parseBTCommands() {
    while (SerialBT.available()) {
        char c = (char)SerialBT.read();
        if (c == '\n' || c == '\r') {
            bt_cmd_buf[bt_cmd_pos] = '\0';
            if (bt_cmd_pos > 0) {
                execCommand(bt_cmd_buf);
            }
            bt_cmd_pos = 0;
        } else if (bt_cmd_pos < 63) {
            bt_cmd_buf[bt_cmd_pos++] = c;
        }
    }
}

void execCommand(const char* cmd) {
    if (strncmp(cmd, "BAND ", 5) == 0) {
        int n = atoi(cmd + 5);
        if (n >= 0 && n < NUM_HF_BANDS) {
            g_band_idx = n;
            prefs.putUChar("band", g_band_idx);
            SerialBT.printf("OK BAND %s\n", HF_BANDS[g_band_idx].name);
        }
    } else if (strcmp(cmd, "STATUS") == 0) {
        sendBTStatus();
    } else if (strcmp(cmd, "CAL START") == 0) {
        bd.startCalibration();
        SerialBT.println("OK CAL STARTED — rotate unit 360° slowly");
    } else if (strcmp(cmd, "CAL END") == 0) {
        bd.finishCalibration();
        SerialBT.println("OK CAL SAVED");
    } else if (strcmp(cmd, "RESET") == 0) {
        SerialBT.println("OK RESTARTING");
        delay(200);
        ESP.restart();
    } else if (strncmp(cmd, "DECL ", 5) == 0) {
        float d = atof(cmd + 5);
        ww.setMagneticDeclination(d);
        prefs.putFloat("decl", d);
        SerialBT.printf("OK DECL %.1f\n", d);
    } else if (strncmp(cmd, "OFFSET ", 7) == 0) {
        float o = atof(cmd + 7);
        ww.setArrayOrientation(o);
        prefs.putFloat("offset", o);
        SerialBT.printf("OK OFFSET %.1f\n", o);
    } else if (strcmp(cmd, "HELP") == 0) {
        SerialBT.println("Commands: BAND n, STATUS, CAL START, CAL END, DECL f, OFFSET f, RESET, HELP");
    } else {
        SerialBT.printf("ERR unknown: %s\n", cmd);
    }
}

// ─── Web Server Handlers ─────────────────────────────────────
static const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html><head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>RF ADF HF</title>
<style>
body{background:#111;color:#0f0;font-family:monospace;text-align:center}
h2{color:#0f0}
#compass{width:260px;height:260px;margin:0 auto}
.btn{background:#222;color:#0f0;border:1px solid #0f0;padding:6px 14px;margin:3px;cursor:pointer;border-radius:3px}
.btn:hover{background:#0f0;color:#000}
#band-grid{display:flex;flex-wrap:wrap;justify-content:center;max-width:320px;margin:0 auto}
#info{font-size:0.9em;margin-top:12px}
</style></head><body>
<h2>RF ADF HF — Watson-Watt</h2>
<svg id="compass" viewBox="0 0 260 260">
  <circle cx="130" cy="130" r="120" fill="none" stroke="#0f0" stroke-width="2"/>
  <circle cx="130" cy="130" r="80" fill="none" stroke="#0a0" stroke-width="1"/>
  <text x="130" y="20" fill="#0f0" text-anchor="middle" font-size="14">N</text>
  <text x="248" y="135" fill="#0f0" text-anchor="middle" font-size="14">E</text>
  <text x="130" y="252" fill="#0f0" text-anchor="middle" font-size="14">S</text>
  <text x="12" y="135" fill="#0f0" text-anchor="middle" font-size="14">W</text>
  <line id="needle" x1="130" y1="130" x2="130" y2="20"
        stroke="#f00" stroke-width="3" stroke-linecap="round"
        transform="rotate(0,130,130)"/>
  <circle cx="130" cy="130" r="6" fill="#f00"/>
  <text id="brg-text" x="130" y="145" fill="#ff0" text-anchor="middle" font-size="16">---</text>
</svg>
<div id="info">SNR: --- | Conf: --- | Band: ---</div>
<div id="gps-info">GPS: ---</div>
<hr style="border-color:#0a0">
<div id="band-grid"></div>
<script>
const bands=["160M","80M","60M","40M","30M","20M","17M","15M","12M","10M","6M"];
const bg=document.getElementById('band-grid');
bands.forEach((b,i)=>{
  const btn=document.createElement('button');
  btn.className='btn'; btn.textContent=b;
  btn.onclick=()=>fetch('/api/band?n='+i,{method:'POST'});
  bg.appendChild(btn);
});
function refresh(){
  fetch('/api/bearing').then(r=>r.json()).then(d=>{
    const angle=d.bearing>=0?d.bearing:0;
    document.getElementById('needle').setAttribute('transform','rotate('+angle+',130,130)');
    document.getElementById('brg-text').textContent=d.bearing>=0?
      d.bearing.toFixed(1)+'°':'---';
    document.getElementById('info').textContent=
      'SNR:'+d.snr.toFixed(1)+'dB | Conf:'+Math.round(d.conf*100)+'% | Band:'+d.band;
    document.getElementById('gps-info').textContent=
      'GPS: '+d.lat.toFixed(4)+', '+d.lon.toFixed(4)+' ('+d.sats+' sats)';
  }).catch(()=>{});
}
setInterval(refresh,500);
</script>
</body></html>
)rawhtml";

void handleRoot() {
    httpServer.send_P(200, "text/html", HTML_PAGE);
}

void handleApiBearing() {
    GPSFix gps = bd.getGPSFix();
    char buf[256];
    snprintf(buf, sizeof(buf),
        "{\"bearing\":%.1f,\"true\":%.1f,\"snr\":%.1f,\"conf\":%.2f,"
        "\"band\":\"%s\",\"lat\":%.5f,\"lon\":%.5f,\"sats\":%d}",
        g_result.bearing_mag,
        (gps.valid && g_result.bearing_mag >= 0)
            ? bd.magneticToTrue(g_result.bearing_mag, gps.lat, gps.lon)
            : g_result.bearing_mag,
        g_result.snr_db, g_result.confidence,
        HF_BANDS[g_band_idx].name,
        gps.lat, gps.lon, gps.satellites);
    httpServer.send(200, "application/json", buf);
}

void handleApiBand() {
    if (httpServer.hasArg("n")) {
        int n = httpServer.arg("n").toInt();
        if (n >= 0 && n < NUM_HF_BANDS) {
            g_band_idx = n;
            prefs.putUChar("band", g_band_idx);
        }
    }
    httpServer.send(200, "text/plain", "OK");
}

void handleApiCal() {
    String action = httpServer.arg("action");
    if (action == "start") {
        bd.startCalibration();
        httpServer.send(200, "application/json", "{\"cal\":\"started\"}");
    } else if (action == "end") {
        bd.finishCalibration();
        httpServer.send(200, "application/json", "{\"cal\":\"saved\"}");
    } else {
        httpServer.send(400, "text/plain", "Bad action");
    }
}

// ─── setup() ────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.println("RF ADF HF v1.0");

    // GPIO init
    pinMode(GPIO_RF_SW_A,  OUTPUT); digitalWrite(GPIO_RF_SW_A, LOW);
    pinMode(GPIO_RF_SW_B,  OUTPUT); digitalWrite(GPIO_RF_SW_B, LOW);
    pinMode(GPIO_RF_SW_EN, OUTPUT); digitalWrite(GPIO_RF_SW_EN, LOW);  // enable
    pinMode(GPIO_BT_LED,   OUTPUT); digitalWrite(GPIO_BT_LED, LOW);
    pinMode(GPIO_SIG_LED,  OUTPUT); digitalWrite(GPIO_SIG_LED, LOW);

    analogSetAttenuation(ADC_ATTENUATION);
    analogSetWidth(12);

    // I2C
    Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL);

    // OLED
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("OLED init failed");
    }
    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(10, 10);
    oled.println("RF ADF HF");
    oled.setTextSize(1);
    oled.setCursor(10, 35);
    oled.println("Watson-Watt");
    oled.setCursor(10, 48);
    oled.println("TM-ADF-001 Rev A");
    oled.display();
    delay(1500);

    // NVS prefs
    prefs.begin(NVS_NAMESPACE, false);
    g_band_idx = prefs.getUChar("band", 3);  // default 40M
    float decl = prefs.getFloat("decl", 0.0f);
    float offs = prefs.getFloat("offset", 0.0f);
    ww.setMagneticDeclination(decl);
    ww.setArrayOrientation(offs);

    // GPS
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPIO_GPS_RX, GPIO_GPS_TX);
    bd.beginGPS(gpsSerial, GPS_BAUD);
    bd.beginCompass(Wire);

    // WiFi AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID);
    Serial.printf("WiFi AP: %s  IP: %s\n", WIFI_SSID, WiFi.softAPIP().toString().c_str());

    // Web server routes
    httpServer.on("/",            HTTP_GET,  handleRoot);
    httpServer.on("/api/bearing", HTTP_GET,  handleApiBearing);
    httpServer.on("/api/band",    HTTP_POST, handleApiBand);
    httpServer.on("/api/cal",     HTTP_POST, handleApiCal);
    httpServer.begin();

    // Bluetooth
    SerialBT.begin(BT_DEVICE_NAME);
    Serial.printf("BT: %s\n", BT_DEVICE_NAME);
    digitalWrite(GPIO_BT_LED, HIGH);

    Serial.println("ADF HF ready.");
}

// ─── loop() ─────────────────────────────────────────────────
void loop() {
    uint32_t now = millis();

    // GPS update (every loop iteration, non-blocking)
    bd.updateGPS();

    // Compass update at ~10 Hz
    static uint32_t last_cmps = 0;
    if (now - last_cmps >= 100) {
        last_cmps = now;
        bd.updateCompass();
    }

    // Bearing measurement
    if (now - g_last_bearing_ms >= BEARING_UPDATE_MS) {
        g_last_bearing_ms = now;
        measureBearing();
        digitalWrite(GPIO_SIG_LED, g_result.bearing_mag >= 0.0f);
    }

    // OLED update
    if (now - g_last_oled_ms >= OLED_UPDATE_MS) {
        g_last_oled_ms = now;
        updateOLED();
    }

    // BT telemetry
    if (now - g_last_bt_ms >= BT_SEND_MS) {
        g_last_bt_ms = now;
        if (SerialBT.hasClient()) sendBTStatus();
    }

    // BT command parsing
    parseBTCommands();

    // Web server
    httpServer.handleClient();
}
