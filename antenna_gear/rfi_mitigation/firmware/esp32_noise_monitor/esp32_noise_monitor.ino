// esp32_noise_monitor.ino — TM-RFI-FW-001 Rev A
// ESP32 RF Noise Floor Monitor
// Scans amateur bands using Si5351 LO + SA612A mixer + AD8307 RSSI
// Logs noise floor to SD card, serves web dashboard, sends to CYD via BT
//
// Dependencies:
//   si5351.h (Etherkit Si5351 library — install via Library Manager)
//   WiFi.h, WebServer.h, BluetoothSerial.h (ESP32 core)
//   SD.h, SPI.h (for optional SD card logging)
//   Wire.h (I2C for Si5351)

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <SPI.h>
#include <SD.h>
#include <si5351.h>
#include "config.h"

// ── Objects ───────────────────────────────────────────────────────────────
Si5351       si5351;
WebServer    server(80);
BluetoothSerial BT;

// ── State ─────────────────────────────────────────────────────────────────
static float    g_noise_dBm[NUM_SCAN_BANDS];          // latest reading per band
static float    g_history[NUM_SCAN_BANDS][HISTORY_DEPTH]; // rolling history
static uint8_t  g_hist_idx = 0;                        // current history slot
static uint8_t  g_current_band = 0;                    // band being scanned now
static bool     g_alert[NUM_SCAN_BANDS];               // threshold exceeded flags
static bool     g_sd_ok = false;
static uint32_t g_scan_count = 0;
static unsigned long g_last_log_ms = 0;
static unsigned long g_band_start_ms = 0;

// ── Read AD8307 RSSI (averaged) ───────────────────────────────────────────
float readRSSI_dBm() {
    uint32_t sum = 0;
    for (int i = 0; i < RSSI_SAMPLES; i++) {
        sum += analogRead(GPIO_RSSI);
        delayMicroseconds(500);
    }
    float counts = (float)sum / RSSI_SAMPLES;
    float v = counts * (3.3f / 4095.0f);
    float dBm = (v / AD8307_SLOPE) + AD8307_INTERCEPT + AD8307_CAL_OFFSET_DB;
    return dBm;
}

// ── Tune Si5351 to LO for given band ─────────────────────────────────────
void tuneToBand(uint8_t band_idx) {
    if (band_idx >= NUM_SCAN_BANDS) return;
    uint64_t f_lo = (uint64_t)(SCAN_BANDS[band_idx].center_hz) + IF_FREQ_HZ;
    // Si5351 library takes frequency in hundredths of Hz
    si5351.set_freq(f_lo * 100ULL, SI5351_CLK0);
    si5351.output_enable(SI5351_CLK0, 1);
}

// ── Check and update alerts ───────────────────────────────────────────────
void checkAlerts() {
    bool any_alert = false;
    for (int i = 0; i < NUM_SCAN_BANDS; i++) {
        g_alert[i] = (g_noise_dBm[i] > SCAN_BANDS[i].threshold_dBm);
        if (g_alert[i]) any_alert = true;
    }
    digitalWrite(GPIO_ALERT_LED, any_alert ? HIGH : LOW);
}

// ── Log to SD card ────────────────────────────────────────────────────────
void logToSD() {
    if (!g_sd_ok) return;
    File f = SD.open(LOG_FILENAME, FILE_APPEND);
    if (!f) return;
    // Header on first write
    if (f.size() == 0) {
        f.print("scan,millis");
        for (int i = 0; i < NUM_SCAN_BANDS; i++)
            f.printf(",%s_dBm", SCAN_BANDS[i].name);
        f.println();
    }
    f.printf("%lu,%lu", (unsigned long)g_scan_count, millis());
    for (int i = 0; i < NUM_SCAN_BANDS; i++)
        f.printf(",%.1f", g_noise_dBm[i]);
    f.println();
    f.close();
}

// ── Build JSON status for web API ────────────────────────────────────────
void buildStatusJSON(String &out) {
    out = "{\"scan\":";
    out += (int)g_scan_count;
    out += ",\"bands\":[";
    for (int i = 0; i < NUM_SCAN_BANDS; i++) {
        if (i > 0) out += ",";
        out += "{\"name\":\"";
        out += SCAN_BANDS[i].name;
        out += "\",\"dBm\":";
        out += String(g_noise_dBm[i], 1);
        out += ",\"alert\":";
        out += g_alert[i] ? "true" : "false";
        out += ",\"history\":[";
        for (int j = 0; j < HISTORY_DEPTH; j++) {
            if (j > 0) out += ",";
            int slot = (g_hist_idx + j) % HISTORY_DEPTH;
            out += String(g_history[i][slot], 1);
        }
        out += "]}";
    }
    out += "]}";
}

// ── Web server: HTML dashboard ────────────────────────────────────────────
static const char HTML_DASH[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html>
<head>
<meta charset='utf-8'>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<title>RF Noise Monitor</title>
<script src='https://cdn.jsdelivr.net/npm/chart.js@4/dist/chart.umd.min.js'></script>
<style>
  body{background:#0a0a1a;color:#e0e0e0;font-family:monospace;margin:0;padding:8px}
  h2{color:#00d4ff;margin:4px 0}
  .grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(120px,1fr));gap:6px;margin:8px 0}
  .band-card{background:#16213e;border:1px solid #0f3460;border-radius:6px;padding:8px;text-align:center}
  .band-card.alert{border-color:#ff4444;background:#2e1010}
  .band-name{font-size:14px;font-weight:bold;color:#00d4ff}
  .band-val{font-size:22px;color:#fff;margin:4px 0}
  .band-bar{height:8px;border-radius:4px;background:#333;margin:4px 0;overflow:hidden}
  .band-fill{height:100%;border-radius:4px;transition:width 0.5s}
  .chart-wrap{background:#16213e;border:1px solid #0f3460;border-radius:6px;padding:8px;margin:8px 0}
  .thresh-ctrl{display:flex;gap:8px;align-items:center;flex-wrap:wrap;margin:8px 0}
  label{font-size:12px;color:#888}
  input[type=number]{width:60px;background:#111;color:#fff;border:1px solid #333;padding:2px}
  button{background:#0f3460;border:1px solid #00d4ff;color:#00d4ff;padding:4px 10px;cursor:pointer;border-radius:3px}
</style>
</head>
<body>
<h2>&#x1F4E1; RF Noise Floor Monitor</h2>
<div class='grid' id='bandGrid'></div>
<div class='thresh-ctrl'>
  <label>Alert threshold (dBm):</label>
  <input type='number' id='thresh' value='-90' min='-130' max='0' step='5'>
  <button onclick='setThresh()'>Set All</button>
  <button onclick='exportCSV()'>Export CSV</button>
  <span id='scan-count' style='color:#888;font-size:11px'></span>
</div>
<div class='chart-wrap'>
  <canvas id='histChart' height='120'></canvas>
</div>
<script>
let chartInst = null;
let allData = {bands:[]};
const COLORS=['#00d4ff','#00ff88','#ffff00','#ff8800','#ff4444','#cc44ff',
              '#44ccff','#88ff44','#ffcc00','#ff6644','#8888ff'];

function barColor(dBm){
  if(dBm>-60) return '#ff4444';
  if(dBm>-80) return '#ffff00';
  return '#00ff88';
}

function renderBands(){
  const g=document.getElementById('bandGrid');
  g.innerHTML='';
  allData.bands.forEach((b,i)=>{
    const pct=Math.max(0,Math.min(100,(b.dBm+120)*100/80));
    const d=document.createElement('div');
    d.className='band-card'+(b.alert?' alert':'');
    d.innerHTML=`<div class='band-name'>${b.name}</div>
      <div class='band-val'>${b.dBm} dBm</div>
      <div class='band-bar'><div class='band-fill' style='width:${pct}%;background:${barColor(b.dBm)}'></div></div>`;
    g.appendChild(d);
  });
  document.getElementById('scan-count').textContent='Scan #'+allData.scan;
}

function renderChart(){
  const labels=Array.from({length:96},(_,i)=>i*2+'s');
  const datasets=allData.bands.map((b,i)=>({
    label:b.name, data:b.history, borderColor:COLORS[i%COLORS.length],
    borderWidth:1.5, pointRadius:0, tension:0.3, fill:false
  }));
  if(chartInst){ chartInst.data.datasets=datasets; chartInst.update('none'); return; }
  chartInst=new Chart(document.getElementById('histChart'),{
    type:'line', data:{labels, datasets},
    options:{animation:false,scales:{
      x:{display:false},
      y:{title:{display:true,text:'dBm',color:'#888'},
         grid:{color:'#222'},ticks:{color:'#888'},min:-130,max:0}
    },plugins:{legend:{labels:{color:'#888',boxWidth:12}}}}
  });
}

async function fetchData(){
  try{
    const r=await fetch('/api/status');
    allData=await r.json();
    renderBands(); renderChart();
  }catch(e){}
}

async function setThresh(){
  const v=document.getElementById('thresh').value;
  await fetch('/api/threshold',{method:'POST',headers:{'Content-Type':'application/json'},
    body:JSON.stringify({threshold_dBm:parseFloat(v)})});
}

function exportCSV(){
  let csv='band,dBm\n';
  allData.bands.forEach(b=>{csv+=b.name+','+b.dBm+'\n';});
  const a=document.createElement('a');
  a.href='data:text/csv;charset=utf-8,'+encodeURIComponent(csv);
  a.download='noise_floor_'+Date.now()+'.csv';
  a.click();
}

fetchData();
setInterval(fetchData, 3000);
</script>
</body>
</html>
)rawhtml";

void handleRoot() { server.send_P(200, "text/html", HTML_DASH); }

void handleStatus() {
    String json;
    buildStatusJSON(json);
    server.send(200, "application/json", json);
}

void handleThreshold() {
    if (!server.hasArg("plain")) { server.send(400); return; }
    String body = server.arg("plain");
    int col = body.indexOf(':');
    if (col >= 0) {
        float t = body.substring(col + 1).toFloat();
        for (int i = 0; i < NUM_SCAN_BANDS; i++)
            const_cast<ScanBand&>(SCAN_BANDS[i]).threshold_dBm = t;
    }
    server.send(200, "text/plain", "OK");
}

void handleReset() {
    memset(g_noise_dBm, 0, sizeof(g_noise_dBm));
    memset(g_history, 0, sizeof(g_history));
    g_scan_count = 0;
    g_hist_idx = 0;
    server.send(200, "text/plain", "OK");
}

// ── BT command handler ────────────────────────────────────────────────────
void processBT(const String &cmd) {
    String c = cmd;
    c.trim(); c.toUpperCase();
    if (c == "STATUS") {
        for (int i = 0; i < NUM_SCAN_BANDS; i++)
            BT.printf("%s:%.1f%s\n", SCAN_BANDS[i].name, g_noise_dBm[i],
                      g_alert[i] ? "!" : "");
    }
    else if (c.startsWith("THRESH ")) {
        float t = c.substring(7).toFloat();
        for (int i = 0; i < NUM_SCAN_BANDS; i++)
            const_cast<ScanBand&>(SCAN_BANDS[i]).threshold_dBm = t;
        BT.printf("OK threshold=%.1f\n", t);
    }
    else if (c == "RESET") {
        memset(g_noise_dBm, 0, sizeof(g_noise_dBm));
        g_scan_count = 0;
        BT.println("OK reset");
    }
    else if (c == "HELP") {
        BT.println("Commands: STATUS, THRESH <dBm>, RESET, HELP");
    }
    else { BT.printf("ERR: %s\n", c.c_str()); }
}

// ── setup() ───────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.printf("\n%s %s\n", FW_VERSION, FW_DATE);

    // GPIO
    pinMode(GPIO_STATUS_LED, OUTPUT);
    pinMode(GPIO_ALERT_LED,  OUTPUT);
    analogSetAttenuation(ADC_11db);
    analogReadResolution(12);

    // Initialize history arrays to −120 dBm (noise floor estimate)
    for (int i = 0; i < NUM_SCAN_BANDS; i++) {
        g_noise_dBm[i] = -120.0f;
        for (int j = 0; j < HISTORY_DEPTH; j++)
            g_history[i][j] = -120.0f;
    }

    // I2C + Si5351
    Wire.begin(21, 22);
    bool si_ok = si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_REF_XTAL, 0);
    if (!si_ok) Serial.println("WARNING: Si5351 init failed — check I2C wiring");
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);

    // SD card (optional)
    SPI.begin(18, 19, 23, GPIO_SD_CS);
    g_sd_ok = SD.begin(GPIO_SD_CS);
    if (g_sd_ok) Serial.println("SD card OK");
    else         Serial.println("SD card not found (logging disabled)");

    // WiFi AP
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    WiFi.softAPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1),
                      IPAddress(255,255,255,0));
    server.on("/",              HTTP_GET,  handleRoot);
    server.on("/api/status",    HTTP_GET,  handleStatus);
    server.on("/api/threshold", HTTP_POST, handleThreshold);
    server.on("/api/reset",     HTTP_POST, handleReset);
    server.begin();
    Serial.printf("Web: http://%s\n", WIFI_IP);

    // Bluetooth
    BT.begin(BT_NAME);
    Serial.printf("BT: %s\n", BT_NAME);

    // Tune to first band
    tuneToBand(0);
    g_band_start_ms = millis();
    Serial.println("Scanning started.");
}

// ── loop() ────────────────────────────────────────────────────────────────
void loop() {
    server.handleClient();

    // BT input
    static String bt_buf;
    while (BT.available()) {
        char ch = BT.read();
        if (ch == '\n' || ch == '\r') {
            if (bt_buf.length() > 0) { processBT(bt_buf); bt_buf = ""; }
        } else bt_buf += ch;
    }

    // Band scanning state machine
    unsigned long now = millis();
    if (now - g_band_start_ms >= SCAN_DWELL_MS) {
        // Read RSSI for current band
        float rssi = readRSSI_dBm();
        g_noise_dBm[g_current_band] = rssi;
        g_history[g_current_band][g_hist_idx] = rssi;

        // Status LED flash
        digitalWrite(GPIO_STATUS_LED, HIGH);
        delay(10);
        digitalWrite(GPIO_STATUS_LED, LOW);

        Serial.printf("[%s] %.1f dBm\n", SCAN_BANDS[g_current_band].name, rssi);

        // Advance to next band
        g_current_band++;
        if (g_current_band >= NUM_SCAN_BANDS) {
            g_current_band = 0;
            g_scan_count++;
            g_hist_idx = (g_hist_idx + 1) % HISTORY_DEPTH;

            // Check alerts
            checkAlerts();

            // BT status broadcast (every full scan cycle)
            if (BT.connected()) {
                String btmsg = "SCAN:" + String(g_scan_count) + ",";
                for (int i = 0; i < NUM_SCAN_BANDS; i++) {
                    btmsg += String(SCAN_BANDS[i].name) + ":" +
                             String(g_noise_dBm[i], 1);
                    if (i < NUM_SCAN_BANDS - 1) btmsg += ",";
                }
                btmsg += "\n";
                BT.print(btmsg);
            }

            // SD logging
            if (g_sd_ok && (now - g_last_log_ms >= (uint32_t)LOG_INTERVAL_MIN * 60000)) {
                g_last_log_ms = now;
                logToSD();
                Serial.println("Logged to SD.");
            }
        }

        // Tune to next band
        tuneToBand(g_current_band);
        g_band_start_ms = now;
    }
}
