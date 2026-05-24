// esp32_sdr_controller.ino — TM-SDR-FW-001 Rev A
// ESP32 SDR Interface Controller
// Controls: preselector BPF, PE4302 attenuator, LNA, upconverter bypass, bias-T
// Interfaces: WiFi AP web server, Bluetooth Serial, USB debug
//
// Dependencies: ESP32 Arduino core 2.x, WiFi.h, WebServer.h, BluetoothSerial.h
//               Adafruit_SSD1306 (optional OLED), Wire.h (for OLED)

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <Preferences.h>
#include <Wire.h>
#include "config.h"
#include "filter_switch.h"

// ── Objects ────────────────────────────────────────────────────────────────
FilterSwitch  fs;
WebServer     server(80);
BluetoothSerial BT;
Preferences   prefs;

// ── State ──────────────────────────────────────────────────────────────────
static unsigned long g_alc_last_ms = 0;
static unsigned long g_rssi_last_ms = 0;
static float         g_rssi_dBm = -99.0f;
static char          g_status_buf[256];

// ── HTML web UI (served from PROGMEM) ─────────────────────────────────────
static const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html>
<head>
<meta charset='utf-8'>
<meta name='viewport' content='width=device-width,initial-scale=1'>
<title>SDR Controller</title>
<style>
  body{background:#1a1a2e;color:#e0e0e0;font-family:monospace;margin:0;padding:8px}
  h2{color:#00d4ff;margin:4px 0}
  .grid{display:grid;grid-template-columns:repeat(4,1fr);gap:4px;margin:8px 0}
  .band-btn{background:#16213e;border:1px solid #0f3460;color:#e0e0e0;
            padding:8px 4px;border-radius:4px;cursor:pointer;font-size:11px;text-align:center}
  .band-btn.active{background:#0f3460;border-color:#00d4ff;color:#00d4ff}
  .band-btn:hover{background:#0f3460}
  .ctrl{display:flex;align-items:center;gap:8px;margin:4px 0}
  .lbl{width:80px;color:#888;font-size:12px}
  input[type=range]{flex:1;accent-color:#00d4ff}
  .toggle{background:#16213e;border:1px solid #0f3460;color:#e0e0e0;
          padding:6px 12px;border-radius:4px;cursor:pointer}
  .toggle.on{background:#0f3460;border-color:#00ff88;color:#00ff88}
  .rssi-bar{height:16px;background:#16213e;border:1px solid #0f3460;border-radius:3px;overflow:hidden}
  .rssi-fill{height:100%;background:linear-gradient(90deg,#00ff88,#ffff00,#ff4444);transition:width 0.3s}
  #rssi-val{color:#00d4ff;font-size:13px}
  .stat{font-size:11px;color:#888;margin:2px 0}
</style>
</head>
<body>
<h2>&#x1F4E1; SDR Interface Controller</h2>

<div class='grid' id='bandGrid'></div>

<div class='ctrl'>
  <span class='lbl'>ATT</span>
  <input type='range' id='att' min='0' max='31' step='1' oninput='setAtt(this.value)'>
  <span id='att-val'>0 dB</span>
</div>

<div class='ctrl'>
  <span class='lbl'>LNA</span>
  <button class='toggle' id='lna-btn' onclick='toggleLNA()'>OFF</button>
  <span class='lbl'>Bypass</span>
  <button class='toggle' id='byp-btn' onclick='toggleBypass()'>OFF</button>
  <span class='lbl'>Bias-T</span>
  <button class='toggle' id='bt-btn' onclick='toggleBiasT()'>OFF</button>
</div>

<div class='ctrl'>
  <span class='lbl'>ALC</span>
  <button class='toggle' id='alc-btn' onclick='toggleALC()'>OFF</button>
  <span style='font-size:11px;color:#888'>Auto level control</span>
</div>

<div class='ctrl'>
  <span class='lbl'>RSSI</span>
  <div class='rssi-bar' style='flex:1'>
    <div class='rssi-fill' id='rssi-fill' style='width:0%'></div>
  </div>
  <span id='rssi-val'>-- dBm</span>
</div>

<div class='stat' id='stat-line'>Connecting...</div>

<script>
const BANDS=['160M','80M','40M','30M','20M','17M','15M','12M','10M','6M','2M','1.25M','70cm','33cm','23cm','WIDE'];
let st={band:'40M',att:0,lna:false,bypass:false,biastee:false,alc:false,rssi:-99};

function buildGrid(){
  const g=document.getElementById('bandGrid');
  BANDS.forEach((b,i)=>{
    const d=document.createElement('div');
    d.className='band-btn'+(b===st.band?' active':'');
    d.id='b'+i; d.textContent=b;
    d.onclick=()=>setBand(b);
    g.appendChild(d);
  });
}

async function fetchStatus(){
  try{
    const r=await fetch('/api/status');
    const j=await r.json();
    st=j; updateUI();
  }catch(e){}
}

function updateUI(){
  BANDS.forEach((b,i)=>{
    const el=document.getElementById('b'+i);
    if(el) el.className='band-btn'+(b===st.band?' active':'');
  });
  document.getElementById('att').value=st.att;
  document.getElementById('att-val').textContent=st.att+' dB';
  btn('lna-btn',st.lna); btn('byp-btn',st.bypass);
  btn('bt-btn',st.biastee); btn('alc-btn',st.alc);
  const pct=Math.max(0,Math.min(100,(st.rssi+80)*100/80));
  document.getElementById('rssi-fill').style.width=pct+'%';
  document.getElementById('rssi-val').textContent=st.rssi.toFixed(1)+' dBm';
  document.getElementById('stat-line').textContent=
    'Band: '+st.band+' | ATT: '+st.att+'dB | LNA:'+(st.lna?'ON':'OFF')+
    ' | Bypass:'+(st.bypass?'ON':'OFF')+' | BiasT:'+(st.biastee?'ON':'OFF');
}

function btn(id,on){
  const el=document.getElementById(id);
  el.className='toggle'+(on?' on':'');
  el.textContent=on?'ON':'OFF';
}

async function setBand(b){await post('/api/band',{band:b}); fetchStatus();}
async function setAtt(v){document.getElementById('att-val').textContent=v+' dB';
  await post('/api/att',{att:parseInt(v)}); }
async function toggleLNA(){await post('/api/lna',{on:!st.lna}); fetchStatus();}
async function toggleBypass(){await post('/api/bypass',{on:!st.bypass}); fetchStatus();}
async function toggleBiasT(){await post('/api/biastee',{on:!st.biastee}); fetchStatus();}
async function toggleALC(){await post('/api/alc',{on:!st.alc}); fetchStatus();}

async function post(url,data){
  await fetch(url,{method:'POST',headers:{'Content-Type':'application/json'},
    body:JSON.stringify(data)});
}

buildGrid();
fetchStatus();
setInterval(fetchStatus, 1000);
</script>
</body>
</html>
)rawhtml";

// ── Web server routes ──────────────────────────────────────────────────────
void handleRoot() {
    server.send_P(200, "text/html", HTML_PAGE);
}

void handleStatus() {
    fs.getStatusJSON(g_status_buf, sizeof(g_status_buf));
    server.send(200, "application/json", g_status_buf);
}

void handleBand() {
    if (!server.hasArg("plain")) { server.send(400, "text/plain", "no body"); return; }
    String body = server.arg("plain");
    // Parse {"band":"40M"}
    int qi = body.indexOf('"', body.indexOf('"') + 1);
    int qe = body.indexOf('"', qi + 1);
    if (qi >= 0 && qe > qi) {
        String band = body.substring(qi + 1, qe);
        fs.selectBandByName(band.c_str());
    }
    server.send(200, "text/plain", "OK");
}

void handleAtt() {
    if (!server.hasArg("plain")) { server.send(400, "text/plain", "no body"); return; }
    String body = server.arg("plain");
    // Parse {"att":10}
    int col = body.indexOf(':');
    if (col >= 0) {
        int val = body.substring(col + 1).toInt();
        fs.setAttenuation((int8_t)val);
    }
    server.send(200, "text/plain", "OK");
}

void handleLNA() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        bool on = body.indexOf("true") >= 0;
        fs.setLNA(on);
    }
    server.send(200, "text/plain", "OK");
}

void handleBypass() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        bool on = body.indexOf("true") >= 0;
        fs.setBypass(on);
    }
    server.send(200, "text/plain", "OK");
}

void handleBiasT() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        bool on = body.indexOf("true") >= 0;
        fs.setBiasT(on);
    }
    server.send(200, "text/plain", "OK");
}

void handleALC() {
    if (server.hasArg("plain")) {
        String body = server.arg("plain");
        bool on = body.indexOf("true") >= 0;
        fs.setALC(on);
    }
    server.send(200, "text/plain", "OK");
}

// ── Bluetooth command parser ───────────────────────────────────────────────
void processBTCommand(const String &cmd) {
    String c = cmd;
    c.trim();
    c.toUpperCase();

    if (c.startsWith("BAND ")) {
        String b = cmd.substring(5);
        b.trim();
        fs.selectBandByName(b.c_str());
        BT.printf("OK BAND %s\n", fs.getBand().name);
    }
    else if (c.startsWith("ATT ")) {
        int v = c.substring(4).toInt();
        fs.setAttenuation(v);
        BT.printf("OK ATT %d\n", fs.getAttenuation());
    }
    else if (c == "LNA ON")  { fs.setLNA(true);  BT.println("OK LNA ON");  }
    else if (c == "LNA OFF") { fs.setLNA(false); BT.println("OK LNA OFF"); }
    else if (c == "BYP ON")  { fs.setBypass(true);  BT.println("OK BYP ON"); }
    else if (c == "BYP OFF") { fs.setBypass(false); BT.println("OK BYP OFF"); }
    else if (c == "BT ON")   { fs.setBiasT(true);  BT.println("OK BT ON"); }
    else if (c == "BT OFF")  { fs.setBiasT(false); BT.println("OK BT OFF"); }
    else if (c == "ALC ON")  { fs.setALC(true);  BT.println("OK ALC ON"); }
    else if (c == "ALC OFF") { fs.setALC(false); BT.println("OK ALC OFF"); }
    else if (c == "STATUS") {
        fs.getStatusBT(g_status_buf, sizeof(g_status_buf));
        BT.print(g_status_buf);
    }
    else if (c == "RSSI") {
        float r = fs.readRSSI_dBm();
        BT.printf("RSSI:%.1f\n", r);
    }
    else if (c == "HELP") {
        BT.println("Commands: BAND <name>, ATT <0-31>, LNA ON/OFF,");
        BT.println("  BYP ON/OFF, BT ON/OFF, ALC ON/OFF, STATUS, RSSI, HELP");
    }
    else {
        BT.printf("ERR unknown: %s\n", c.c_str());
    }
}

// ── Serial (USB) command parser ────────────────────────────────────────────
void processSerial(const String &cmd) {
    // Mirror BT handling but to Serial
    String c = cmd;
    c.trim();
    c.toUpperCase();
    if (c.startsWith("BAND ")) {
        String b = cmd.substring(5); b.trim();
        fs.selectBandByName(b.c_str());
        Serial.printf("OK BAND %s\n", fs.getBand().name);
    } else if (c.startsWith("ATT ")) {
        fs.setAttenuation(c.substring(4).toInt());
        Serial.printf("OK ATT %d\n", fs.getAttenuation());
    } else if (c == "STATUS") {
        fs.getStatusJSON(g_status_buf, sizeof(g_status_buf));
        Serial.println(g_status_buf);
    } else if (c == "RSSI") {
        Serial.printf("RSSI:%.1f dBm\n", fs.readRSSI_dBm());
    } else if (c == "LNA ON")  { fs.setLNA(true);  Serial.println("OK"); }
    else if (c == "LNA OFF")   { fs.setLNA(false); Serial.println("OK"); }
    else if (c == "BYP ON")    { fs.setBypass(true);  Serial.println("OK"); }
    else if (c == "BYP OFF")   { fs.setBypass(false); Serial.println("OK"); }
    else if (c == "ALC ON")    { fs.setALC(true);  Serial.println("OK"); }
    else if (c == "ALC OFF")   { fs.setALC(false); Serial.println("OK"); }
}

// ── setup() ───────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    Serial.printf("\n%s %s\n", FW_VERSION, FW_DATE);

    // Load saved preferences
    prefs.begin("sdr", false);
    uint8_t saved_band = prefs.getUChar("band", 2);
    int8_t  saved_att  = prefs.getChar("att", 0);
    bool    saved_lna  = prefs.getBool("lna", false);
    prefs.end();

    // Initialize hardware
    fs.begin();
    fs.selectBand(saved_band);
    fs.setAttenuation(saved_att);
    fs.setLNA(saved_lna);

    // WiFi AP
    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    WiFi.softAPConfig(IPAddress(192, 168, 4, 1),
                      IPAddress(192, 168, 4, 1),
                      IPAddress(255, 255, 255, 0));
    Serial.printf("WiFi AP: %s  IP: %s\n", WIFI_SSID, WIFI_IP);

    // Web server routes
    server.on("/",          HTTP_GET,  handleRoot);
    server.on("/api/status",HTTP_GET,  handleStatus);
    server.on("/api/band",  HTTP_POST, handleBand);
    server.on("/api/att",   HTTP_POST, handleAtt);
    server.on("/api/lna",   HTTP_POST, handleLNA);
    server.on("/api/bypass",HTTP_POST, handleBypass);
    server.on("/api/biastee",HTTP_POST,handleBiasT);
    server.on("/api/alc",   HTTP_POST, handleALC);
    server.begin();

    // Bluetooth
    BT.begin(BT_NAME);
    Serial.printf("Bluetooth: %s\n", BT_NAME);

    Serial.printf("Init complete. Band: %s  ATT: %d dB  LNA: %s\n",
        fs.getBand().name, fs.getAttenuation(), fs.getLNA() ? "ON" : "OFF");
}

// ── loop() ────────────────────────────────────────────────────────────────
void loop() {
    server.handleClient();

    // Bluetooth input
    static String bt_buf;
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n' || c == '\r') {
            if (bt_buf.length() > 0) {
                processBTCommand(bt_buf);
                bt_buf = "";
            }
        } else {
            bt_buf += c;
        }
    }

    // USB Serial input
    static String ser_buf;
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (ser_buf.length() > 0) {
                processSerial(ser_buf);
                ser_buf = "";
            }
        } else {
            ser_buf += c;
        }
    }

    // ALC polling
    unsigned long now = millis();
    if (now - g_alc_last_ms >= ALC_POLL_MS) {
        g_alc_last_ms = now;
        fs.updateALC();
    }

    // RSSI periodic send to BT (every 2 seconds if BT connected)
    if (now - g_rssi_last_ms >= 2000) {
        g_rssi_last_ms = now;
        g_rssi_dBm = fs.getLastRSSI();
        if (BT.connected()) {
            // Send compact status packet to CYD display
            fs.getStatusBT(g_status_buf, sizeof(g_status_buf));
            BT.print(g_status_buf);
        }
    }
}
