// =============================================================================
// esp32_hf_tuner.ino — Main Firmware for HF L/T/Pi Antenna Tuner
// Project:  RF-HFTuner / Motorized Antenna Tuner Suite
// Device:   ESP32-WROOM-32
// Rev:      1.0  2026-05-01
//
// Libraries required (Arduino Library Manager):
//   AccelStepper         >= 1.64.0  (Mike McCauley)
//   BluetoothSerial      (ESP32 built-in, esp32 board package)
//   WiFi                 (ESP32 built-in)
//   WebServer            (ESP32 built-in)
//   ArduinoJson          >= 6.21.0  (Benoit Blanchon)
//   Preferences          (ESP32 built-in)
//   Adafruit_SSD1306     >= 2.5.7   (Adafruit)
//   Adafruit GFX Library >= 1.11.9  (Adafruit)
//   Wire                 (ESP32 built-in)
//
// Pinout: see config.h
// =============================================================================

#include <Arduino.h>
#include <AccelStepper.h>
#include <BluetoothSerial.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "config.h"
#include "swr.h"
#include "tuner.h"

// =============================================================================
// GLOBAL OBJECTS
// =============================================================================

// Stepper motors (AccelStepper, DRIVER mode: external step/dir)
AccelStepper stepperCap(AccelStepper::DRIVER, PIN_CAP_STEP, PIN_CAP_DIR);
AccelStepper stepperInd(AccelStepper::DRIVER, PIN_IND_STEP, PIN_IND_DIR);

// SWR measurement
SWRMeter swrMeter;

// Auto-tuner
AutoTuner autoTuner;

// Position managers
CapPositionManager    capMgr;
InductorPositionManager indMgr;

// Network (relay) controller
NetworkController netCtrl;

// Command parser
TunerCommandParser cmdParser;

// Band preset storage
BandPresetStore presetStore;

// Bluetooth Serial (SPP)
BluetoothSerial SerialBT;

// Web server on port 80
WebServer webServer(HTTP_PORT);

// Non-volatile storage
Preferences prefs;

// OLED display (SSD1306, 128×64, I2C)
Adafruit_SSD1306 oled(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// =============================================================================
// STATE VARIABLES
// =============================================================================

FirmwareState firmwareState = STATE_INIT;
int           currentBand   = BAND_40M;
uint32_t      lastSWRUpdate = 0;
uint32_t      lastOLEDUpdate = 0;
uint32_t      lastStatusBroadcast = 0;
uint32_t      lastLEDToggle = 0;
bool          ledState      = false;
float         cachedSWR     = 0.0f;
float         cachedFwdW    = 0.0f;
String        btBuffer      = "";
String        serialBuffer  = "";
bool          homingDone    = false;

// =============================================================================
// HTML CONTROL PAGE (stored in PROGMEM to save heap)
// =============================================================================

static const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>RF-HFTuner Control</title>
<style>
  body { background:#1a1a2e; color:#eee; font-family:monospace; margin:0; padding:10px; }
  h1   { color:#00d4ff; text-align:center; font-size:1.4em; margin-bottom:5px; }
  .subtitle { text-align:center; color:#888; font-size:0.8em; margin-bottom:15px; }
  .card { background:#16213e; border:1px solid #0f3460; border-radius:8px; padding:12px; margin-bottom:12px; }
  .card h2 { color:#00d4ff; font-size:1em; margin:0 0 10px 0; }
  .swr-display { text-align:center; font-size:3em; font-weight:bold; }
  .swr-good    { color:#00ff88; }
  .swr-warn    { color:#ffcc00; }
  .swr-bad     { color:#ff4444; }
  .swr-bar-bg  { background:#333; height:20px; border-radius:10px; overflow:hidden; margin:8px 0; }
  .swr-bar     { height:100%; border-radius:10px; transition:width 0.4s, background 0.4s; }
  .info-grid   { display:grid; grid-template-columns:1fr 1fr; gap:8px; }
  .info-item   { background:#0f3460; border-radius:5px; padding:8px; text-align:center; }
  .info-label  { font-size:0.7em; color:#888; }
  .info-value  { font-size:1.2em; color:#00d4ff; }
  .band-grid   { display:grid; grid-template-columns:repeat(4,1fr); gap:5px; }
  .band-btn    { background:#0f3460; border:1px solid #00d4ff; color:#eee; padding:8px 4px;
                 border-radius:5px; cursor:pointer; font-size:0.85em; text-align:center; }
  .band-btn:hover   { background:#00d4ff; color:#000; }
  .band-btn.active  { background:#00ff88; color:#000; border-color:#00ff88; }
  .btn-row     { display:flex; gap:8px; flex-wrap:wrap; }
  .btn         { padding:10px 18px; border:none; border-radius:5px; cursor:pointer;
                 font-family:monospace; font-size:0.95em; font-weight:bold; }
  .btn-tune    { background:#00d4ff; color:#000; flex:2; }
  .btn-home    { background:#ffcc00; color:#000; }
  .btn-stop    { background:#ff4444; color:#fff; }
  .btn-save    { background:#00ff88; color:#000; }
  .btn:hover   { opacity:0.85; }
  .btn:active  { opacity:0.7; transform:scale(0.97); }
  .network-sel { display:flex; gap:6px; flex-wrap:wrap; }
  .net-btn     { flex:1; padding:8px; background:#0f3460; border:1px solid #555;
                 color:#eee; border-radius:5px; cursor:pointer; font-family:monospace; }
  .net-btn.active { background:#e94560; border-color:#e94560; color:#fff; }
  .log-box     { background:#000; border:1px solid #333; border-radius:5px; padding:8px;
                 height:80px; overflow-y:auto; font-size:0.75em; color:#00ff88; }
  .status-bar  { text-align:center; color:#888; font-size:0.75em; padding:5px 0; }
  input[type=range] { width:100%; accent-color:#00d4ff; }
  .range-row   { display:flex; align-items:center; gap:8px; }
  .range-lbl   { min-width:100px; color:#aaa; font-size:0.85em; }
  .range-val   { min-width:70px; color:#00d4ff; text-align:right; }
</style>
</head>
<body>
<h1>RF-HFTuner</h1>
<div class="subtitle">Motorized HF Antenna Tuner &mdash; <span id="ip-addr">192.168.4.1</span></div>

<!-- SWR DISPLAY -->
<div class="card">
  <h2>SWR Meter</h2>
  <div class="swr-display" id="swr-val">--.-</div>
  <div class="swr-bar-bg">
    <div class="swr-bar" id="swr-bar" style="width:0%;background:#00ff88;"></div>
  </div>
  <div class="info-grid">
    <div class="info-item">
      <div class="info-label">FWD Power</div>
      <div class="info-value" id="fwd-w">-- W</div>
    </div>
    <div class="info-item">
      <div class="info-label">Refl Power</div>
      <div class="info-value" id="rev-w">-- W</div>
    </div>
    <div class="info-item">
      <div class="info-label">Return Loss</div>
      <div class="info-value" id="rl-db">-- dB</div>
    </div>
    <div class="info-item">
      <div class="info-label">Gamma</div>
      <div class="info-value" id="gamma-val">--</div>
    </div>
  </div>
</div>

<!-- TUNING STATE -->
<div class="card">
  <h2>Tuner State</h2>
  <div class="info-grid">
    <div class="info-item">
      <div class="info-label">Inductance</div>
      <div class="info-value" id="l-val">-- &micro;H</div>
    </div>
    <div class="info-item">
      <div class="info-label">Capacitance</div>
      <div class="info-value" id="c-val">-- pF</div>
    </div>
    <div class="info-item">
      <div class="info-label">Band</div>
      <div class="info-value" id="band-val">--</div>
    </div>
    <div class="info-item">
      <div class="info-label">Network</div>
      <div class="info-value" id="net-val">--</div>
    </div>
    <div class="info-item" style="grid-column:span 2;">
      <div class="info-label">State</div>
      <div class="info-value" id="state-val">--</div>
    </div>
  </div>
</div>

<!-- MANUAL L/C SLIDERS -->
<div class="card">
  <h2>Manual L/C Control</h2>
  <div class="range-row">
    <span class="range-lbl">Inductance:</span>
    <input type="range" id="l-slider" min="0" max="3000" value="0"
           oninput="updateLSlider(this.value)">
    <span class="range-val" id="l-slider-val">0.0 &micro;H</span>
  </div>
  <div class="range-row" style="margin-top:8px;">
    <span class="range-lbl">Capacitance:</span>
    <input type="range" id="c-slider" min="10" max="500" value="10"
           oninput="updateCSlider(this.value)">
    <span class="range-val" id="c-slider-val">10 pF</span>
  </div>
  <div class="btn-row" style="margin-top:8px;">
    <button class="btn" style="background:#555;color:#eee;" onclick="applyLC()">Apply L/C</button>
  </div>
</div>

<!-- BAND SELECT -->
<div class="card">
  <h2>Band Select</h2>
  <div class="band-grid">
    <div class="band-btn" onclick="selectBand('160m',0)">160m</div>
    <div class="band-btn" onclick="selectBand('80m',1)">80m</div>
    <div class="band-btn" onclick="selectBand('60m',2)">60m</div>
    <div class="band-btn" onclick="selectBand('40m',3)">40m</div>
    <div class="band-btn" onclick="selectBand('30m',4)">30m</div>
    <div class="band-btn" onclick="selectBand('20m',5)">20m</div>
    <div class="band-btn" onclick="selectBand('17m',6)">17m</div>
    <div class="band-btn" onclick="selectBand('15m',7)">15m</div>
    <div class="band-btn" onclick="selectBand('12m',8)">12m</div>
    <div class="band-btn" onclick="selectBand('10m',9)">10m</div>
    <div class="band-btn" onclick="selectBand('6m',10)" style="grid-column:span 2;">6m (50 MHz)</div>
  </div>
</div>

<!-- NETWORK TOPOLOGY -->
<div class="card">
  <h2>Network Topology</h2>
  <div class="network-sel">
    <button class="net-btn" onclick="setNetwork(0)" id="net-llp">L Low-pass</button>
    <button class="net-btn" onclick="setNetwork(1)" id="net-lhp">L High-pass</button>
    <button class="net-btn" onclick="setNetwork(2)" id="net-pi">Pi</button>
    <button class="net-btn" onclick="setNetwork(3)" id="net-t">T</button>
  </div>
</div>

<!-- ACTION BUTTONS -->
<div class="card">
  <h2>Actions</h2>
  <div class="btn-row">
    <button class="btn btn-tune" onclick="doTune()">AUTO TUNE</button>
    <button class="btn btn-home" onclick="doHome()">HOME</button>
    <button class="btn btn-stop" onclick="doStop()">STOP</button>
    <button class="btn btn-save" onclick="doSave()">SAVE</button>
  </div>
  <div class="btn-row" style="margin-top:8px;">
    <button class="btn" style="background:#555;color:#eee;flex:1;" onclick="doCalibrate()">CALIBRATE SWR</button>
    <button class="btn" style="background:#555;color:#eee;flex:1;" onclick="loadPreset()">LOAD PRESET</button>
  </div>
</div>

<!-- LOG -->
<div class="card">
  <h2>Log</h2>
  <div class="log-box" id="log-box"></div>
</div>

<div class="status-bar">RF-HFTuner v1.0 &mdash; Updated: <span id="last-update">--</span></div>

<script>
var activeBand = -1;
var activeNet  = -1;

function log(msg) {
  var lb = document.getElementById('log-box');
  var t = new Date().toTimeString().slice(0,8);
  lb.innerHTML += '['+t+'] '+msg+'<br>';
  lb.scrollTop = lb.scrollHeight;
}

function updateSWRDisplay(swr) {
  var el = document.getElementById('swr-val');
  var bar = document.getElementById('swr-bar');
  var s = swr >= 99 ? '&#x221e;' : swr.toFixed(2);
  el.textContent = '';
  el.innerHTML = s;
  el.className = 'swr-display ' + (swr <= 1.5 ? 'swr-good' : swr <= 2.5 ? 'swr-warn' : 'swr-bad');
  // Bar: map SWR 1.0=100% green, 1.5=70% yellow, 3.0=0% red
  var pct = Math.max(0, Math.min(100, (3.0 - Math.min(swr,3.0)) / 2.0 * 100));
  var col = swr <= 1.5 ? '#00ff88' : swr <= 2.5 ? '#ffcc00' : '#ff4444';
  bar.style.width = pct + '%';
  bar.style.background = col;
}

function fetchStatus() {
  fetch('/status').then(r=>r.json()).then(d=>{
    updateSWRDisplay(d.swr || 99);
    document.getElementById('fwd-w').textContent = (d.fwd_w||0).toFixed(1)+' W';
    document.getElementById('rev-w').textContent = (d.rev_w||0).toFixed(1)+' W';
    document.getElementById('rl-db').textContent = (d.return_loss||0).toFixed(1)+' dB';
    document.getElementById('gamma-val').textContent = (d.gamma||0).toFixed(3);
    document.getElementById('l-val').innerHTML = (d.L_uh||0).toFixed(2)+'&nbsp;&micro;H';
    document.getElementById('c-val').textContent = (d.C_pf||0).toFixed(0)+' pF';
    document.getElementById('band-val').textContent = d.band_name || '--';
    document.getElementById('net-val').textContent = d.network || '--';
    document.getElementById('state-val').textContent = d.state || '--';
    document.getElementById('last-update').textContent = new Date().toTimeString().slice(0,8);
    // Highlight active band button
    if(d.band_index !== undefined && d.band_index !== activeBand) {
      if(activeBand >= 0) document.querySelectorAll('.band-btn')[activeBand].classList.remove('active');
      activeBand = d.band_index;
      if(activeBand >= 0) document.querySelectorAll('.band-btn')[activeBand].classList.add('active');
    }
    // Highlight active network
    var nets = ['net-llp','net-lhp','net-pi','net-t'];
    nets.forEach(function(id){ document.getElementById(id).classList.remove('active'); });
    var netMap = {'L_LOWPASS':0,'L_HIGHPASS':1,'PI_NETWORK':2,'T_NETWORK':3};
    if(d.network && netMap[d.network] !== undefined) {
      document.getElementById(nets[netMap[d.network]]).classList.add('active');
    }
  }).catch(function(e){ /* ignore poll errors */ });
}

function selectBand(name, idx) {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({band: idx, auto_tune: false})
  }).then(r=>r.json()).then(d=>{ log('Band: '+name+' — '+d.message); });
}

function doTune() {
  log('Starting auto-tune...');
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({band: activeBand >= 0 ? activeBand : 3, auto_tune: true})
  }).then(r=>r.json()).then(d=>{ log('Tune: '+d.message); });
}

function doHome() {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({action: 'home'})
  }).then(r=>r.json()).then(d=>{ log('Home: '+d.message); });
}

function doStop() {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({action: 'stop'})
  }).then(r=>r.json()).then(d=>{ log('Stop: '+d.message); });
}

function doSave() {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({action: 'save', band: activeBand >= 0 ? activeBand : 3})
  }).then(r=>r.json()).then(d=>{ log('Save: '+d.message); });
}

function doCalibrate() {
  fetch('/cal', {method:'POST'}).then(r=>r.json()).then(d=>{ log('Cal: '+d.message); });
}

function loadPreset() {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({action: 'load', band: activeBand >= 0 ? activeBand : 3})
  }).then(r=>r.json()).then(d=>{ log('Load: '+d.message); });
}

function setNetwork(n) {
  fetch('/tune', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({action: 'network', network: n})
  }).then(r=>r.json()).then(d=>{ log('Network: '+d.message); });
}

function updateLSlider(v) {
  var uh = (v / 100.0).toFixed(2);
  document.getElementById('l-slider-val').innerHTML = uh + ' &micro;H';
}

function updateCSlider(v) {
  document.getElementById('c-slider-val').textContent = v + ' pF';
}

function applyLC() {
  var l = (document.getElementById('l-slider').value / 100.0).toFixed(2);
  var c = document.getElementById('c-slider').value;
  fetch('/setlc', {method:'POST', headers:{'Content-Type':'application/json'},
    body: JSON.stringify({L_uh: parseFloat(l), C_pf: parseFloat(c)})
  }).then(r=>r.json()).then(d=>{ log('SetLC: L='+l+' C='+c+' — '+d.message); });
}

// Poll status every 500ms
setInterval(fetchStatus, 500);
fetchStatus();
log('RF-HFTuner web interface ready.');
</script>
</body>
</html>
)rawhtml";

// =============================================================================
// SETUP
// =============================================================================

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n=== RF-HFTuner v" FW_VERSION_STRING " ===");
    Serial.println("Device: " DEVICE_NAME);

    // --- Status LED ---
    pinMode(PIN_LED_STATUS, OUTPUT);
    digitalWrite(PIN_LED_STATUS, HIGH);

    // --- Limit switch GPIO (input-only, external pull-ups) ---
    pinMode(PIN_CAP_MIN, INPUT);
    pinMode(PIN_CAP_MAX, INPUT);
    pinMode(PIN_IND_MIN, INPUT);
    pinMode(PIN_IND_MAX, INPUT);

    // --- Band selector GPIO ---
    pinMode(PIN_BAND_BIT0, INPUT_PULLDOWN);
    pinMode(PIN_BAND_BIT1, INPUT_PULLDOWN);
    pinMode(PIN_BAND_BIT2, INPUT_PULLDOWN);
    pinMode(PIN_BAND_BIT3, INPUT_PULLDOWN);

    // --- NVS ---
    prefs.begin(NVS_NAMESPACE_CONFIG, false);
    currentBand = prefs.getInt(NVS_KEY_LAST_BAND, BAND_40M);
    Serial.print("Restoring band: ");
    Serial.println(BAND_TABLE[currentBand].name);

    // --- I2C & OLED ---
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("WARN: OLED not found");
    } else {
        oled.clearDisplay();
        oled.setTextSize(1);
        oled.setTextColor(SSD1306_WHITE);
        oled.setCursor(0, 0);
        oled.println("RF-HFTuner v" FW_VERSION_STRING);
        oled.println("Initializing...");
        oled.display();
    }

    // --- Capacitor stepper ---
    stepperCap.setMaxSpeed(2000.0f);
    stepperCap.setAcceleration(500.0f);
    stepperCap.setEnablePin(PIN_CAP_EN);
    stepperCap.setPinsInverted(false, false, true); // EN active-low
    stepperCap.disableOutputs();

    // --- Inductor stepper ---
    stepperInd.setMaxSpeed(2000.0f);
    stepperInd.setAcceleration(500.0f);
    stepperInd.setEnablePin(PIN_IND_EN);
    stepperInd.setPinsInverted(false, false, true);
    stepperInd.disableOutputs();

    // --- Position managers ---
    capMgr.begin(&stepperCap);
    indMgr.begin(&stepperInd);

    // --- Network controller ---
    netCtrl.begin();
    netCtrl.setNetwork(BAND_TABLE[currentBand].preferred_net);

    // --- SWR meter ---
    swrMeter.begin(PIN_VFWD_ADC, PIN_VREV_ADC);
    swrMeter.loadCalibration();

    // --- Auto-tuner ---
    autoTuner.begin(&swrMeter, &capMgr, &indMgr);

    // --- Band preset store ---
    presetStore.begin(&prefs);

    // --- Command parser ---
    cmdParser.begin(&swrMeter, &autoTuner, &capMgr, &indMgr, &netCtrl, &prefs);

    // --- Bluetooth SPP ---
    SerialBT.begin(BT_DEVICE_NAME);
    Serial.println("BT SPP started: " BT_DEVICE_NAME);

    // --- WiFi AP ---
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL, 0, WIFI_MAX_CLIENTS);
    WiFi.softAPConfig(
        IPAddress(192, 168, 4, 1),
        IPAddress(192, 168, 4, 1),
        IPAddress(255, 255, 255, 0)
    );
    Serial.print("WiFi AP: "); Serial.print(WIFI_SSID);
    Serial.print(" IP: "); Serial.println(WiFi.softAPIP());

    // --- Web server routes ---
    webServer.on("/",        HTTP_GET,  handleRoot);
    webServer.on("/status",  HTTP_GET,  handleStatus);
    webServer.on("/tune",    HTTP_POST, handleTune);
    webServer.on("/setlc",   HTTP_POST, handleSetLC);
    webServer.on("/bands",   HTTP_GET,  handleBands);
    webServer.on("/cal",     HTTP_POST, handleCal);
    webServer.onNotFound([]() {
        webServer.send(404, "text/plain", "Not found");
    });
    webServer.begin();
    Serial.println("HTTP server started");

    // --- Home sequence ---
    Serial.println("Starting home sequence...");
    oledShow("Homing...", "", "", "");
    firmwareState = STATE_HOMING;
    bool capHomed = capMgr.homeToMin();
    bool indHomed = indMgr.homeToMin();
    if (capHomed && indHomed) {
        homingDone = true;
        firmwareState = STATE_IDLE;
        Serial.println("Homing complete.");
        // Load last preset
        float last_L, last_C, last_swr;
        if (presetStore.load(currentBand, &last_L, &last_C, &last_swr)) {
            capMgr.moveToPF(last_C);
            indMgr.moveToUH(last_L);
            Serial.printf("Restored band %s: L=%.2f µH, C=%.0f pF\n",
                          BAND_TABLE[currentBand].name, last_L, last_C);
        }
    } else {
        firmwareState = STATE_FAULT;
        Serial.println("FAULT: Homing failed!");
    }

    oledUpdate();
    digitalWrite(PIN_LED_STATUS, LOW);
    Serial.println("Setup complete. Entering main loop.");
}

// =============================================================================
// LOOP
// =============================================================================

void loop() {
    uint32_t now = millis();

    // --- Run steppers (AccelStepper non-blocking) ---
    stepperCap.run();
    stepperInd.run();
    capMgr.update();
    indMgr.update();

    // --- Auto-tuner state machine ---
    if (autoTuner.isTuning()) {
        autoTuner.update();
        if (!autoTuner.isTuning()) {
            // Tuning just finished
            TuningResult res = autoTuner.getResult();
            if (res.success) {
                firmwareState = STATE_IDLE;
                presetStore.save(res.band, res.L_uh, res.C_pf, res.swr);
                prefs.putInt(NVS_KEY_LAST_BAND, currentBand);
                Serial.printf("AutoTune OK: SWR=%.2f L=%.2f C=%.0f\n",
                              res.swr, res.L_uh, res.C_pf);
            } else {
                firmwareState = STATE_FAULT;
                Serial.println("AutoTune FAULT");
            }
        }
    }

    // --- SWR polling ---
    if (now - lastSWRUpdate >= SWR_UPDATE_MS) {
        lastSWRUpdate = now;
        cachedSWR   = swrMeter.readSWR();
        cachedFwdW  = swrMeter.readForward();
        if (cachedSWR > SAFETY_MAX_SWR_FAULT && firmwareState == STATE_IDLE) {
            // High SWR detected; don't fault if tuning is in progress
        }
    }

    // --- Serial commands ---
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialBuffer.length() > 0) {
                String resp = cmdParser.process(serialBuffer);
                Serial.println(resp);
                serialBuffer = "";
            }
        } else {
            serialBuffer += c;
        }
    }

    // --- Bluetooth commands ---
    while (SerialBT.available()) {
        char c = (char)SerialBT.read();
        if (c == '\n' || c == '\r') {
            if (btBuffer.length() > 0) {
                String resp = cmdParser.process(btBuffer);
                SerialBT.println(resp);
                btBuffer = "";
            }
        } else {
            btBuffer += c;
        }
    }

    // --- Web server ---
    webServer.handleClient();

    // --- OLED update ---
    if (now - lastOLEDUpdate >= OLED_UPDATE_MS) {
        lastOLEDUpdate = now;
        oledUpdate();
    }

    // --- LED blink ---
    ledBlink(now);
}

// =============================================================================
// LED BLINK HANDLER
// =============================================================================

void ledBlink(uint32_t now) {
    uint32_t elapsed = now - lastLEDToggle;
    uint32_t onTime, offTime;

    switch (firmwareState) {
        case STATE_TUNING:
        case STATE_HOMING:
            onTime = LED_TUNING_ON_MS; offTime = LED_TUNING_OFF_MS; break;
        case STATE_FAULT:
            // Rapid double-blink pattern handled simply here
            onTime = LED_FAULT_ON_MS; offTime = LED_FAULT_OFF_MS; break;
        case STATE_IDLE:
        default:
            onTime = LED_IDLE_ON_MS; offTime = LED_IDLE_OFF_MS; break;
    }

    if (ledState && elapsed >= onTime) {
        ledState = false;
        digitalWrite(PIN_LED_STATUS, LOW);
        lastLEDToggle = now;
    } else if (!ledState && elapsed >= offTime) {
        ledState = true;
        digitalWrite(PIN_LED_STATUS, HIGH);
        lastLEDToggle = now;
    }
}

// =============================================================================
// OLED DISPLAY UPDATE
// =============================================================================

void oledShow(const char* l1, const char* l2, const char* l3, const char* l4) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.setCursor(0, 0);  oled.println(l1);
    oled.setCursor(0, 16); oled.println(l2);
    oled.setCursor(0, 32); oled.println(l3);
    oled.setCursor(0, 48); oled.println(l4);
    oled.display();
}

void oledUpdate() {
    char line1[22], line2[22], line3[22], line4[22];
    // Line 1: Band + frequency
    snprintf(line1, sizeof(line1), "%s %.3fMHz",
             BAND_TABLE[currentBand].name,
             BAND_TABLE[currentBand].freq_mhz);
    // Line 2: SWR
    if (cachedSWR >= 99.0f)
        snprintf(line2, sizeof(line2), "SWR: ---  Fwd:%.0fW", cachedFwdW);
    else
        snprintf(line2, sizeof(line2), "SWR: %.2f  Fwd:%.0fW", cachedSWR, cachedFwdW);
    // Line 3: L and C
    snprintf(line3, sizeof(line3), "L:%.2fuH C:%.0fpF",
             indMgr.getCurrentUH(), capMgr.getCurrentPF());
    // Line 4: State + network
    const char* stateStr;
    switch (firmwareState) {
        case STATE_INIT:     stateStr = "INIT";    break;
        case STATE_HOMING:   stateStr = "HOMING";  break;
        case STATE_IDLE:     stateStr = "TUNED";   break;
        case STATE_TUNING:   stateStr = "TUNING";  break;
        case STATE_MOVING:   stateStr = "MOVING";  break;
        case STATE_CALIBRATE:stateStr = "CAL";     break;
        case STATE_FAULT:    stateStr = "FAULT";   break;
        default:             stateStr = "???";     break;
    }
    snprintf(line4, sizeof(line4), "%s [%s]",
             stateStr, netCtrl.getNetworkName());
    oledShow(line1, line2, line3, line4);
}

// =============================================================================
// WEB SERVER HANDLERS
// =============================================================================

void handleRoot() {
    webServer.sendHeader("Cache-Control", "no-cache");
    webServer.send_P(200, "text/html", HTML_PAGE);
}

void handleStatus() {
    StaticJsonDocument<512> doc;
    doc["swr"]         = swrMeter.readSWR();
    doc["fwd_w"]       = swrMeter.readForward();
    doc["rev_w"]       = swrMeter.readReverse();
    doc["return_loss"] = swrMeter.readReturnLoss();
    doc["gamma"]       = swrMeter.readGamma();
    doc["L_uh"]        = indMgr.getCurrentUH();
    doc["C_pf"]        = capMgr.getCurrentPF();
    doc["band_index"]  = currentBand;
    doc["band_name"]   = BAND_TABLE[currentBand].name;
    doc["network"]     = netCtrl.getNetworkName();
    doc["state"]       = (firmwareState == STATE_IDLE)   ? "IDLE" :
                         (firmwareState == STATE_TUNING) ? "TUNING" :
                         (firmwareState == STATE_HOMING) ? "HOMING" :
                         (firmwareState == STATE_FAULT)  ? "FAULT" : "BUSY";
    doc["uptime_ms"]   = millis();
    doc["fw_version"]  = FW_VERSION_STRING;
    doc["is_tuning"]   = autoTuner.isTuning();
    doc["homed"]       = (capMgr.isHomed() && indMgr.isHomed());

    String body;
    serializeJson(doc, body);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", body);
}

void handleTune() {
    if (!webServer.hasArg("plain")) {
        webServer.send(400, "application/json", "{\"message\":\"No body\"}");
        return;
    }
    StaticJsonDocument<256> req;
    DeserializationError err = deserializeJson(req, webServer.arg("plain"));
    if (err) {
        webServer.send(400, "application/json", "{\"message\":\"JSON error\"}");
        return;
    }

    StaticJsonDocument<128> resp;
    const char* action = req["action"] | "";

    if (strcmp(action, "home") == 0) {
        firmwareState = STATE_HOMING;
        capMgr.homeToMin();
        indMgr.homeToMin();
        firmwareState = STATE_IDLE;
        resp["message"] = "Homing started";
    } else if (strcmp(action, "stop") == 0) {
        autoTuner.stop();
        stepperCap.stop();
        stepperInd.stop();
        firmwareState = STATE_IDLE;
        resp["message"] = "Stopped";
    } else if (strcmp(action, "save") == 0) {
        int band = req["band"] | currentBand;
        presetStore.save(band, indMgr.getCurrentUH(), capMgr.getCurrentPF(), swrMeter.readSWR());
        resp["message"] = "Preset saved";
    } else if (strcmp(action, "load") == 0) {
        int band = req["band"] | currentBand;
        float L, C, swr;
        if (presetStore.load(band, &L, &C, &swr)) {
            capMgr.moveToPF(C);
            indMgr.moveToUH(L);
            resp["message"] = "Preset loaded";
        } else {
            resp["message"] = "No preset";
        }
    } else if (strcmp(action, "network") == 0) {
        int n = req["network"] | 0;
        netCtrl.setNetwork((NetworkType)n);
        resp["message"] = "Network set";
    } else {
        // auto_tune or band select
        int band = req["band"] | currentBand;
        if (band >= 0 && band < NUM_BANDS) currentBand = band;
        netCtrl.setNetwork(BAND_TABLE[currentBand].preferred_net);
        bool doAutoTune = req["auto_tune"] | false;
        if (doAutoTune) {
            firmwareState = STATE_TUNING;
            if (!autoTuner.startAutoTune(currentBand)) {
                firmwareState = STATE_FAULT;
                resp["message"] = "Tune start failed";
            } else {
                resp["message"] = "Auto-tune started";
            }
        } else {
            // Just select band, load preset if available
            float L, C, swr;
            if (presetStore.load(currentBand, &L, &C, &swr)) {
                capMgr.moveToPF(C);
                indMgr.moveToUH(L);
            }
            resp["message"] = "Band selected";
        }
    }

    String body;
    serializeJson(resp, body);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", body);
}

void handleSetLC() {
    if (!webServer.hasArg("plain")) {
        webServer.send(400, "application/json", "{\"message\":\"No body\"}");
        return;
    }
    StaticJsonDocument<128> req;
    deserializeJson(req, webServer.arg("plain"));
    float L = req["L_uh"] | indMgr.getCurrentUH();
    float C = req["C_pf"] | capMgr.getCurrentPF();
    capMgr.moveToPF(C);
    indMgr.moveToUH(L);
    StaticJsonDocument<64> resp;
    resp["message"] = "LC set";
    String body;
    serializeJson(resp, body);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", body);
}

void handleBands() {
    StaticJsonDocument<1024> doc;
    JsonArray arr = doc.to<JsonArray>();
    for (int i = 0; i < NUM_BANDS; i++) {
        JsonObject b = arr.createNestedObject();
        b["index"]    = i;
        b["name"]     = BAND_TABLE[i].name;
        b["freq_mhz"] = BAND_TABLE[i].freq_mhz;
        float L, C, swr;
        b["has_preset"] = presetStore.load(i, &L, &C, &swr);
        if (presetStore.hasPreset(i)) {
            b["preset_swr"] = presetStore.getBestSWR(i);
        }
    }
    String body;
    serializeJson(doc, body);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", body);
}

void handleCal() {
    swrMeter.calibrate();
    swrMeter.saveCalibration();
    StaticJsonDocument<64> resp;
    resp["message"] = "Calibration done";
    String body;
    serializeJson(resp, body);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", body);
}

// =============================================================================
// END OF esp32_hf_tuner.ino
// =============================================================================
