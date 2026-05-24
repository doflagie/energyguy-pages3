// ============================================================
// esp32_adf_vhf.ino — TM-ADF-002 Rev A
// Doppler VHF/UHF Automatic Direction Finder
// ============================================================

#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Preferences.h>

#include "config.h"
#include "doppler.h"
// Reuse BearingDisplay from hf project (GPS/compass identical)
// For standalone use: include bearing.h copy or paste relevant class

DopplerRDF       doppler;
WebServer        httpServer(HTTP_PORT);
BluetoothSerial  SerialBT;
HardwareSerial   gpsSerial(GPS_UART_NUM);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);
Preferences      prefs;

// ─── State ──────────────────────────────────────────────────
static DopplerResult g_result;
static uint8_t       g_band_idx = 0;   // default 2M
static uint32_t      g_last_bearing_ms = 0;
static uint32_t      g_last_oled_ms    = 0;
static uint32_t      g_last_bt_ms      = 0;

// Audio ring buffer (double-buffered)
static volatile int16_t  adc_buf_a[DOPPLER_BLOCK_SIZE];
static volatile int16_t  adc_buf_b[DOPPLER_BLOCK_SIZE];
static volatile uint16_t adc_idx    = 0;
static volatile bool     buf_ready  = false;
static volatile bool     use_buf_a  = true;

// ─── Timer ISR for ADC sampling ─────────────────────────────
hw_timer_t* adcTimer = nullptr;

void IRAM_ATTR onAdcTimer() {
    int16_t sample = (int16_t)analogRead(GPIO_AUDIO_ADC) - 2048;
    if (use_buf_a) {
        adc_buf_a[adc_idx++] = sample;
        if (adc_idx >= DOPPLER_BLOCK_SIZE) {
            adc_idx   = 0;
            buf_ready = true;
            use_buf_a = false;
        }
    } else {
        adc_buf_b[adc_idx++] = sample;
        if (adc_idx >= DOPPLER_BLOCK_SIZE) {
            adc_idx   = 0;
            buf_ready = true;
            use_buf_a = true;
        }
    }
}

// ─── OLED update ────────────────────────────────────────────
void updateOLED() {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);

    oled.setCursor(0, 0);
    oled.printf("ADF VHF  %s", VHF_BANDS[g_band_idx].name);

    oled.setCursor(0, 16);
    if (g_result.valid) {
        const char* dirs[] = {"N","NE","E","SE","S","SW","W","NW"};
        int di = (int)((g_result.bearing_mag + 22.5f) / 45.0f) % 8;
        oled.printf("BRG: %05.1f\xb0 %s", g_result.bearing_mag, dirs[di]);
    } else {
        oled.print("BRG: --- (no sig)");
    }

    oled.setCursor(0, 32);
    oled.printf("RSSI:%6.1fdBm", g_result.rssi_dbm);

    oled.setCursor(0, 48);
    oled.printf("CNF:%3.0f%%  ERR:%.1f\xb0",
                g_result.confidence * 100.0f,
                g_result.phase_error_deg);

    oled.display();
}

// ─── BT output ──────────────────────────────────────────────
void sendBTStatus() {
    SerialBT.printf(
        "BRG:%.1f,TRUE:%.1f,RSSI:%.1f,CNF:%.2f,BAND:%s\n",
        g_result.bearing_mag,
        g_result.bearing_true,
        g_result.rssi_dbm,
        g_result.confidence,
        VHF_BANDS[g_band_idx].name
    );
}

// ─── BT command parser ───────────────────────────────────────
static char bt_buf[64]; static uint8_t bt_pos = 0;
void parseBT() {
    while (SerialBT.available()) {
        char c = SerialBT.read();
        if (c == '\n') {
            bt_buf[bt_pos] = '\0';
            if (bt_pos > 0) {
                if (strncmp(bt_buf, "BAND ", 5) == 0) {
                    int n = atoi(bt_buf + 5);
                    if (n >= 0 && n < NUM_VHF_BANDS) { g_band_idx = n; }
                    SerialBT.printf("OK BAND %s\n", VHF_BANDS[g_band_idx].name);
                } else if (strcmp(bt_buf, "STATUS") == 0) {
                    sendBTStatus();
                } else if (strcmp(bt_buf, "CAL START") == 0) {
                    // Enter calibration mode: user transmits from known direction
                    SerialBT.println("OK CAL — transmit from known bearing, then CAL SAVE <deg>");
                } else if (strncmp(bt_buf, "CAL SAVE ", 9) == 0) {
                    float known_brg = atof(bt_buf + 9);
                    // Record current measured phase vs known bearing
                    int16_t* active = use_buf_a ? (int16_t*)adc_buf_b : (int16_t*)adc_buf_a;
                    float mag, phase;
                    doppler.goertzel(active, DOPPLER_BLOCK_SIZE, DOPPLER_ADC_HZ,
                                     DOPPLER_SWITCH_HZ, mag, phase);
                    doppler.recordCalibrationPoint(known_brg, phase);
                    SerialBT.printf("OK CAL point saved (known=%.1f, phase=%.1f)\n", known_brg, phase);
                } else if (strcmp(bt_buf, "CAL END") == 0) {
                    doppler.computeCalibration();
                    SerialBT.printf("OK CAL done, offset=%.1f\n", doppler.getCalibrationOffset());
                } else if (strncmp(bt_buf, "SQUELCH ", 8) == 0) {
                    SerialBT.println("OK (squelch not implemented in this version)");
                } else {
                    SerialBT.println("ERR unknown");
                }
            }
            bt_pos = 0;
        } else if (bt_pos < 63) {
            bt_buf[bt_pos++] = c;
        }
    }
}

// ─── Web handlers ───────────────────────────────────────────
static const char VHF_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>RF ADF VHF</title>
<style>
body{background:#111;color:#0f0;font-family:monospace;text-align:center}
svg{display:block;margin:0 auto}
.btn{background:#222;color:#0f0;border:1px solid #0f0;padding:6px 12px;margin:3px;cursor:pointer}
.btn:hover{background:#0f0;color:#000}
#rssi-bar{height:16px;background:#030;border:1px solid #0f0;width:240px;margin:6px auto;position:relative}
#rssi-fill{height:100%;background:#0f0;width:0%}
</style></head><body>
<h2>RF ADF VHF — Doppler</h2>
<svg width="260" height="260" viewBox="0 0 260 260">
  <circle cx="130" cy="130" r="120" fill="none" stroke="#0f0" stroke-width="2"/>
  <text x="130" y="18" fill="#0f0" text-anchor="middle" font-size="13">N</text>
  <text x="248" y="134" fill="#0f0" text-anchor="middle" font-size="13">E</text>
  <text x="130" y="252" fill="#0f0" text-anchor="middle" font-size="13">S</text>
  <text x="12" y="134" fill="#0f0" text-anchor="middle" font-size="13">W</text>
  <line id="needle" x1="130" y1="130" x2="130" y2="16"
        stroke="#f00" stroke-width="3" stroke-linecap="round"
        transform="rotate(0,130,130)"/>
  <circle cx="130" cy="130" r="5" fill="#f00"/>
  <text id="brg-lbl" x="130" y="148" fill="#ff0" text-anchor="middle" font-size="15">---</text>
</svg>
<div id="rssi-bar"><div id="rssi-fill"></div></div>
<div id="stats">RSSI: --- | Conf: --- | Band: ---</div>
<div id="band-grid" style="max-width:320px;margin:8px auto;display:flex;flex-wrap:wrap;justify-content:center"></div>
<script>
['2M','1.25M','70CM','33CM','20CM'].forEach((b,i)=>{
  const bt=document.createElement('button');
  bt.className='btn'; bt.textContent=b;
  bt.onclick=()=>fetch('/api/band?n='+i,{method:'POST'});
  document.getElementById('band-grid').appendChild(bt);
});
function refresh(){
  fetch('/api/status').then(r=>r.json()).then(d=>{
    if(d.valid){
      document.getElementById('needle').setAttribute('transform','rotate('+d.bearing+',130,130)');
      document.getElementById('brg-lbl').textContent=d.bearing.toFixed(1)+'°';
    }
    const pct=Math.min(100,Math.max(0,(d.rssi+100)*1.5));
    document.getElementById('rssi-fill').style.width=pct+'%';
    document.getElementById('stats').textContent=
      'RSSI:'+d.rssi.toFixed(0)+'dBm Conf:'+Math.round(d.conf*100)+'% Band:'+d.band;
  }).catch(()=>{});
}
setInterval(refresh,500);
</script></body></html>
)rawhtml";

void handleRoot()    { httpServer.send_P(200, "text/html", VHF_HTML); }
void handleStatus() {
    char buf[256];
    snprintf(buf, sizeof(buf),
        "{\"bearing\":%.1f,\"true\":%.1f,\"rssi\":%.1f,\"conf\":%.2f,\"valid\":%s,\"band\":\"%s\"}",
        g_result.bearing_mag, g_result.bearing_true, g_result.rssi_dbm,
        g_result.confidence, g_result.valid ? "true" : "false",
        VHF_BANDS[g_band_idx].name);
    httpServer.send(200, "application/json", buf);
}
void handleBand() {
    if (httpServer.hasArg("n")) {
        int n = httpServer.arg("n").toInt();
        if (n >= 0 && n < NUM_VHF_BANDS) g_band_idx = n;
    }
    httpServer.send(200, "text/plain", "OK");
}

// ─── setup() ────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);

    // GPIO
    analogSetAttenuation(ADC_ATTEN_DB_11);
    analogSetWidth(12);
    pinMode(GPIO_BT_LED, OUTPUT);

    // I2C + OLED
    Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL);
    oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR);
    oled.clearDisplay();
    oled.setTextSize(2); oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(5, 10); oled.println("ADF VHF");
    oled.setTextSize(1);  oled.setCursor(5, 40); oled.println("Doppler RDF");
    oled.display(); delay(1200);

    // Doppler init
    doppler.begin(GPIO_SW_A0, GPIO_SW_A1, GPIO_SW_ENABLE, DOPPLER_SWITCH_HZ);

    // Reference output — 1 kHz square wave on GPIO_REF_OUT
    ledcSetup(0, DOPPLER_SWITCH_HZ, 8);   // channel 0, 1kHz, 8-bit
    ledcAttachPin(GPIO_REF_OUT, 0);
    ledcWrite(0, 128);   // 50% duty

    // ADC timer ISR
    adcTimer = timerBegin(0, 80, true);   // 80MHz / 80 = 1MHz tick
    timerAttachInterrupt(adcTimer, &onAdcTimer, true);
    timerAlarmWrite(adcTimer, 1000000 / DOPPLER_ADC_HZ, true);  // 100µs = 10kHz
    timerAlarmEnable(adcTimer);

    // WiFi AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID);

    // Web server
    httpServer.on("/",           HTTP_GET,  handleRoot);
    httpServer.on("/api/status", HTTP_GET,  handleStatus);
    httpServer.on("/api/band",   HTTP_POST, handleBand);
    httpServer.begin();

    // Bluetooth
    SerialBT.begin(BT_DEVICE_NAME);
    digitalWrite(GPIO_BT_LED, HIGH);

    Serial.println("ADF VHF ready.");
}

// ─── loop() ─────────────────────────────────────────────────
void loop() {
    uint32_t now = millis();

    // Process audio buffer when ready
    if (buf_ready) {
        buf_ready = false;
        // use the buffer that was just completed (opposite of current)
        const int16_t* proc = use_buf_a ? (int16_t*)adc_buf_b : (int16_t*)adc_buf_a;
        g_result = doppler.compute(proc, DOPPLER_BLOCK_SIZE, DOPPLER_ADC_HZ);
    }

    // OLED refresh
    if (now - g_last_oled_ms >= 250) {
        g_last_oled_ms = now;
        updateOLED();
    }

    // BT telemetry
    if (now - g_last_bt_ms >= 1000) {
        g_last_bt_ms = now;
        if (SerialBT.hasClient()) sendBTStatus();
    }

    parseBT();
    httpServer.handleClient();
}
