/**
 * esp32_cal_master.ino — TM-CAL-FW-001
 * GPS-Disciplined Field Calibration Station
 *
 * Functions:
 *   - GPS discipline loop: corrects Si5351 to GPS 1PPS accuracy
 *   - Frequency counter: 1 Hz – 400 MHz, GPS-gated
 *   - Signal source: programmable calibration output (8 kHz – 160 MHz)
 *   - Power meter: AD8307 RSSI, calibrated absolute level
 *   - Web UI: band/freq selection, counter display, level readout
 *   - BT serial: CYD status packet
 *   - NVS: saves correction, cal offset, last frequency
 */

#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include "BluetoothSerial.h"
#include <Preferences.h>
#include <si5351.h>          // etherkit/si5351arduino library
#include <TinyGPS++.h>       // mikalhart/TinyGPSPlus library
#include "config.h"

// ── Objects ────────────────────────────────────────────────────────────────────
Si5351       si5351;
TinyGPSPlus  gps;
WebServer    server(80);
BluetoothSerial BT;
Preferences  prefs;
HardwareSerial gpsSerial(2);  // UART2

// ── GPS / Discipline State ─────────────────────────────────────────────────────
volatile uint32_t g_pps_count      = 0;
volatile bool     g_pps_flag       = false;
volatile int64_t  g_pcnt_at_pps[2] = {0, 0};  // captured counter values at PPS
volatile uint8_t  g_pps_phase      = 0;

bool     g_gps_fix       = false;
int      g_gps_sats      = 0;
float    g_gps_lat       = 0.0f, g_gps_lon = 0.0f;
String   g_gps_time      = "00:00:00";
bool     g_disc_running  = false;
bool     g_disc_locked   = false;
int      g_lock_consec   = 0;
float    g_freq_error_ppb = 0.0f;
int32_t  g_correction    = 0;   // Si5351 correction in 0.01 ppb units

// ── Frequency Counter State ────────────────────────────────────────────────────
bool     g_cnt_running   = false;
uint64_t g_cnt_result    = 0;   // most recent count (raw, multiply by prescaler)
int      g_cnt_gate      = COUNTER_GATE_SEC;
bool     g_cnt_div10     = false;
uint64_t g_cnt_freq_hz   = 0;   // displayed frequency

// ── Signal Source State ────────────────────────────────────────────────────────
uint64_t g_prog_freq_cHz = 1000000000ULL;  // 10 MHz default (centiherts)
uint8_t  g_prog_preset   = 2;              // index into FREQ_PRESETS

// ── Power Meter ────────────────────────────────────────────────────────────────
float    g_rssi_raw_dBm  = -99.0f;
float    g_rssi_cal_dBm  = -99.0f;
float    g_cal_offset_dB = 0.0f;

// ── Calibration Log ────────────────────────────────────────────────────────────
struct CalEntry {
    char     timestamp[20];
    char     instrument[16];
    float    measured_val;
    float    expected_val;
    float    error_val;
    char     units[8];
};
CalEntry g_cal_log[CAL_LOG_ENTRIES];
uint8_t  g_cal_log_idx = 0;
uint8_t  g_cal_log_count = 0;

// ── Hardware Timer / PCNT ─────────────────────────────────────────────────────
#include "driver/pcnt.h"
#include "driver/timer.h"

volatile int32_t  g_pcnt_overflow = 0;
volatile uint64_t g_pcnt_total    = 0;
volatile uint8_t  g_gate_pps      = 0;
volatile bool     g_gate_active   = false;

void IRAM_ATTR pcnt_overflow_isr(void *arg) {
    g_pcnt_overflow++;
    PCNT.int_clr.val = BIT(PCNT_UNIT_0);
}

void IRAM_ATTR pps_isr() {
    g_pps_count++;
    g_pps_flag = true;

    if (!g_gate_active) {
        // Start gate
        pcnt_counter_clear(PCNT_UNIT_0);
        g_pcnt_overflow = 0;
        g_gate_pps = 0;
        g_gate_active = true;
    } else {
        g_gate_pps++;
        if (g_gate_pps >= (uint8_t)g_cnt_gate) {
            // End gate: latch count
            int16_t cnt_val = 0;
            pcnt_get_counter_value(PCNT_UNIT_0, &cnt_val);
            g_pcnt_total = (uint64_t)(uint32_t)g_pcnt_overflow * 32768ULL + (uint16_t)cnt_val;
            g_gate_active = false;
            g_cnt_running = false;
        }
    }
}

void setupPCNT() {
    pcnt_config_t cfg = {
        .pulse_gpio_num  = GPIO_PCNT_INPUT,
        .ctrl_gpio_num   = PCNT_PIN_NOT_USED,
        .lctrl_mode      = PCNT_MODE_KEEP,
        .hctrl_mode      = PCNT_MODE_KEEP,
        .pos_mode        = PCNT_COUNT_INC,
        .neg_mode        = PCNT_COUNT_DIS,
        .counter_h_lim   = 32767,
        .counter_l_lim   = -1,
        .unit            = PCNT_UNIT_0,
        .channel         = PCNT_CHANNEL_0,
    };
    pcnt_unit_config(&cfg);
    pcnt_set_filter_value(PCNT_UNIT_0, 2);  // filter <2 APB clocks (80 MHz → 25 ns min pulse)
    pcnt_filter_enable(PCNT_UNIT_0);
    pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM);
    pcnt_isr_register(pcnt_overflow_isr, NULL, 0, NULL);
    pcnt_intr_enable(PCNT_UNIT_0);
    pcnt_counter_pause(PCNT_UNIT_0);
    pcnt_counter_clear(PCNT_UNIT_0);
    pcnt_counter_resume(PCNT_UNIT_0);
}

// ── Si5351 Setup ──────────────────────────────────────────────────────────────
void setupSi5351() {
    prefs.begin("cal_master", false);
    g_correction    = prefs.getLong("si_corr", 0);
    g_cal_offset_dB = prefs.getFloat(CAL_OFFSET_KEY, 0.0f);
    g_prog_freq_cHz = prefs.getULong64("prog_freq", 1000000000ULL);
    prefs.end();

    Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL);
    bool ok = si5351.init(SI5351_CRYSTAL_LOAD_8PF, SI5351_XTAL_FREQ, g_correction);
    if (!ok) Serial.println("[CAL] Si5351 init FAILED");

    si5351.set_freq(SI5351_CLK0_FREQ * 100ULL, SI5351_CLK0);
    si5351.set_freq(SI5351_CLK1_FREQ * 100ULL, SI5351_CLK1);
    si5351.set_freq(g_prog_freq_cHz, SI5351_CLK2);
    si5351.drive_strength(SI5351_CLK0, SI5351_CLK0_MA);
    si5351.drive_strength(SI5351_CLK1, SI5351_CLK1_MA);
    si5351.drive_strength(SI5351_CLK2, SI5351_CLK2_MA);
    si5351.output_enable(SI5351_CLK0, 1);
    si5351.output_enable(SI5351_CLK1, 1);
    si5351.output_enable(SI5351_CLK2, 1);
}

// ── GPS Discipline Loop ───────────────────────────────────────────────────────
void runDisciplineLoop() {
    if (!g_gps_fix) return;
    if (g_cnt_running) return;   // counter in use; skip this window

    // Start a 10s counter gate on CLK0 (10 MHz)
    // PPS ISR handles gating; g_pcnt_total is filled after 10 PPS intervals
    g_cnt_gate    = GATE_SECONDS;
    g_cnt_div10   = false;
    digitalWrite(GPIO_RANGE_SEL, HIGH);
    // Reconfigure PCNT input to GPIO32 (direct)
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = GPIO_PCNT_INPUT;
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;
    cfg.pos_mode       = PCNT_COUNT_INC;
    cfg.neg_mode       = PCNT_COUNT_DIS;
    cfg.lctrl_mode     = PCNT_MODE_KEEP;
    cfg.hctrl_mode     = PCNT_MODE_KEEP;
    cfg.counter_h_lim  = 32767;
    cfg.counter_l_lim  = -1;
    cfg.unit           = PCNT_UNIT_0;
    cfg.channel        = PCNT_CHANNEL_0;
    pcnt_unit_config(&cfg);

    g_cnt_running = true;
    g_gate_active = false;  // ISR will start gate on next PPS

    // Wait (non-blocking via flag)
    uint32_t t0 = millis();
    while (g_cnt_running && (millis() - t0 < (GATE_SECONDS + 2) * 1000UL)) {
        delay(10);
        feedGPS();
    }
    if (g_cnt_running) { g_cnt_running = false; return; }  // timeout

    int64_t error_counts = (int64_t)g_pcnt_total - (int64_t)EXPECTED_COUNTS;
    g_freq_error_ppb = (float)error_counts / ((float)GATE_SECONDS * 0.01f);
    // (error_counts / (10 MHz × GATE_SECONDS)) × 1e9 = error_counts / GATE_SECONDS × 100 ppb per count
    // Simplified: 1 count in 100M over 10s = 0.1 count/s = 0.01 ppb. So ppb = counts / (10 * 0.01) wait...
    // Actually: error_ppb = (error_counts / EXPECTED_COUNTS) * 1e9
    //         = error_counts / 100000000 * 1e9 = error_counts * 10 ppb/count
    g_freq_error_ppb = (float)error_counts * 10.0f / (float)GATE_SECONDS;

    g_disc_running = true;
    float correction_ppb = -g_freq_error_ppb * DISCIPLINE_KP;
    int32_t correction_reg = (int32_t)(correction_ppb * 100.0f);  // 0.01 ppb units
    g_correction += correction_reg;
    g_correction = constrain(g_correction, -MAX_CORRECTION, MAX_CORRECTION);

    si5351.set_correction(g_correction, SI5351_PLL_INPUT_XO);

    // Lock detection
    if (fabsf(g_freq_error_ppb) < LOCK_PPB_THRESH) {
        g_lock_consec++;
        if (g_lock_consec >= LOCK_CONSEC_WIN) {
            g_disc_locked = true;
            // Save correction to NVS
            prefs.begin("cal_master", false);
            prefs.putLong("si_corr", g_correction);
            prefs.end();
        }
    } else {
        g_lock_consec = 0;
        g_disc_locked = false;
    }
}

// ── Frequency Counter (user-facing) ──────────────────────────────────────────
void startCounter(bool div10, int gate_s) {
    g_cnt_div10   = div10;
    g_cnt_gate    = gate_s;
    g_cnt_running = true;
    g_gate_active = false;
    g_pcnt_total  = 0;

    // Reconfigure PCNT input pin
    pcnt_config_t cfg = {};
    cfg.pulse_gpio_num = div10 ? GPIO_PCNT_DIV10 : GPIO_PCNT_INPUT;
    cfg.ctrl_gpio_num  = PCNT_PIN_NOT_USED;
    cfg.pos_mode       = PCNT_COUNT_INC;
    cfg.neg_mode       = PCNT_COUNT_DIS;
    cfg.lctrl_mode     = PCNT_MODE_KEEP;
    cfg.hctrl_mode     = PCNT_MODE_KEEP;
    cfg.counter_h_lim  = 32767;
    cfg.counter_l_lim  = -1;
    cfg.unit           = PCNT_UNIT_0;
    cfg.channel        = PCNT_CHANNEL_0;
    pcnt_unit_config(&cfg);
    digitalWrite(GPIO_RANGE_SEL, div10 ? LOW : HIGH);
}

void updateCounterResult() {
    if (g_cnt_running) return;
    uint64_t raw = g_pcnt_total;
    uint64_t multiplier = g_cnt_div10 ? 10ULL : 1ULL;
    g_cnt_freq_hz = raw * multiplier / (uint64_t)g_cnt_gate;
}

// ── AD8307 Power Meter ────────────────────────────────────────────────────────
void readRSSI() {
    uint32_t sum = 0;
    for (int i = 0; i < RSSI_AVG_SAMPLES; i++) {
        sum += analogRead(GPIO_RSSI_ADC);
        delayMicroseconds(100);
    }
    float v_mv = ((float)sum / RSSI_AVG_SAMPLES) * (RSSI_VREF_MV / RSSI_ADC_BITS);
    g_rssi_raw_dBm = (v_mv / RSSI_SLOPE_MV_DB) + RSSI_INTERCEPT;
    g_rssi_cal_dBm = g_rssi_raw_dBm + g_cal_offset_dB;
}

// ── GPS Feed ──────────────────────────────────────────────────────────────────
void feedGPS() {
    while (gpsSerial.available()) {
        char c = gpsSerial.read();
        gps.encode(c);
    }
    if (gps.location.isUpdated()) {
        g_gps_fix  = gps.location.isValid();
        g_gps_sats = gps.satellites.value();
        g_gps_lat  = gps.location.lat();
        g_gps_lon  = gps.location.lng();
    }
    if (gps.time.isUpdated() && gps.time.isValid()) {
        char buf[12];
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
                 gps.time.hour(), gps.time.minute(), gps.time.second());
        g_gps_time = String(buf);
    }
}

// ── LED Management ────────────────────────────────────────────────────────────
void updateLEDs() {
    static uint32_t t_last = 0;
    static bool blink_state = false;
    if (millis() - t_last > 500) {
        blink_state = !blink_state;
        t_last = millis();
    }
    // GPS LED
    if (!g_gps_fix)      digitalWrite(GPIO_LED_GPS, blink_state);  // blink = searching
    else                 digitalWrite(GPIO_LED_GPS, HIGH);          // solid = fix

    // Lock LED
    if (g_disc_locked)   digitalWrite(GPIO_LED_LOCK, HIGH);        // solid = locked
    else if (g_disc_running) digitalWrite(GPIO_LED_LOCK, blink_state); // blink = disciplining
    else                 digitalWrite(GPIO_LED_LOCK, LOW);          // off = no GPS
}

// ── Bluetooth Status Packet ───────────────────────────────────────────────────
void sendBTPacket() {
    char buf[160];
    snprintf(buf, sizeof(buf),
             "GPS:%d,SATS:%d,TIME:%s,LOCK:%d,ERR:%.2f,CORR:%ld,"
             "CLK2:%.0f,RSSI:%.1f,CNT:%.0f\n",
             (int)g_gps_fix, g_gps_sats, g_gps_time.c_str(),
             (int)g_disc_locked, g_freq_error_ppb, (long)g_correction,
             (double)(g_prog_freq_cHz / 100ULL),
             g_rssi_cal_dBm,
             (double)g_cnt_freq_hz);
    BT.print(buf);
}

// ── Web Server HTML ───────────────────────────────────────────────────────────
static const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html><html><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Cal Station</title>
<style>
  body{font-family:monospace;background:#1a1a1a;color:#e0e0e0;margin:0;padding:8px}
  h2{color:#4fc3f7;margin:4px 0}
  .card{background:#2a2a2a;border-radius:6px;padding:10px;margin:6px 0}
  .row{display:flex;gap:10px;flex-wrap:wrap}
  .cell{flex:1;min-width:120px}
  label{color:#90a4ae;font-size:.85em}
  .val{font-size:1.3em;color:#80cbc4}
  .locked{color:#66bb6a}
  .unlocked{color:#ef5350}
  button{background:#37474f;color:#e0e0e0;border:none;border-radius:4px;
         padding:8px 12px;margin:2px;cursor:pointer;font-size:.9em}
  button:hover{background:#546e7a}
  button.active{background:#1565c0}
  select,input{background:#37474f;color:#e0e0e0;border:1px solid #555;
               border-radius:4px;padding:4px;margin:2px}
</style></head>
<body>
<h2>GPS CALIBRATION STATION</h2>
<div class="card">
  <div class="row">
    <div class="cell"><label>GPS STATUS</label><br><span id="gps_st" class="val">--</span></div>
    <div class="cell"><label>SATELLITES</label><br><span id="sats" class="val">--</span></div>
    <div class="cell"><label>UTC TIME</label><br><span id="gps_time" class="val">--:--:--</span></div>
    <div class="cell"><label>DISCIPLINE</label><br><span id="lock_st" class="val">--</span></div>
    <div class="cell"><label>ERROR (ppb)</label><br><span id="err_ppb" class="val">--</span></div>
    <div class="cell"><label>CORRECTION</label><br><span id="corr" class="val">--</span></div>
  </div>
</div>
<div class="card">
  <b>SIGNAL SOURCE (CLK2 — PROG OUT)</b>
  <div class="row" style="margin-top:6px">
    <div class="cell">
      <label>PRESET</label><br>
      <select id="preset_sel" onchange="setPreset(this.value)"></select>
    </div>
    <div class="cell">
      <label>CUSTOM FREQ (Hz)</label><br>
      <input type="number" id="custom_freq" min="8000" max="160000000" value="10000000">
      <button onclick="setCustom()">SET</button>
    </div>
    <div class="cell"><label>RSSI (dBm)</label><br><span id="rssi" class="val">--</span></div>
  </div>
</div>
<div class="card">
  <b>FREQUENCY COUNTER</b>
  <div class="row" style="margin-top:6px">
    <div class="cell">
      <label>RANGE</label><br>
      <button id="btn_direct" class="active" onclick="setRange(false)">DIRECT (≤40M)</button>
      <button id="btn_div10" onclick="setRange(true)">÷10 (≤400M)</button>
    </div>
    <div class="cell">
      <label>GATE</label><br>
      <button onclick="setGate(1)">1s</button>
      <button onclick="setGate(10)">10s</button>
    </div>
    <div class="cell">
      <button onclick="startCount()">▶ START COUNT</button>
    </div>
    <div class="cell"><label>MEASURED FREQ</label><br><span id="cnt_freq" class="val">-- Hz</span></div>
  </div>
</div>
<div class="card">
  <b>CALIBRATION LOG</b> <button onclick="clearLog()">CLEAR</button>
  <button onclick="window.location='/api/callog'">EXPORT CSV</button>
  <div id="log_div" style="max-height:150px;overflow-y:auto;font-size:.8em;margin-top:4px"></div>
</div>
<script>
const presets=[
  {l:"100 kHz",f:100000},{l:"1 MHz",f:1000000},{l:"10 MHz",f:10000000},
  {l:"14.1 MHz",f:14100000},{l:"25 MHz",f:25000000},{l:"50 MHz",f:50000000},
  {l:"100 MHz",f:100000000},{l:"144 MHz",f:144000000}];
let range_div10=false,gate_s=1;

window.onload=function(){
  const s=document.getElementById('preset_sel');
  presets.forEach((p,i)=>{const o=document.createElement('option');o.value=i;o.text=p.l;s.add(o);});
  s.value=2;  // 10 MHz default
  poll();
};
function poll(){fetch('/api/status').then(r=>r.json()).then(d=>{
  document.getElementById('gps_st').textContent=d.gps_fix?"FIX":"NO FIX";
  document.getElementById('sats').textContent=d.sats;
  document.getElementById('gps_time').textContent=d.time;
  document.getElementById('lock_st').innerHTML='<span class="'+(d.locked?"locked":"unlocked")+'">'+(d.locked?"LOCKED":"DISC")+'</span>';
  document.getElementById('err_ppb').textContent=d.err_ppb.toFixed(2);
  document.getElementById('corr').textContent=d.correction;
  document.getElementById('rssi').textContent=d.rssi_dBm.toFixed(1)+' dBm';
  document.getElementById('cnt_freq').textContent=fmtFreq(d.cnt_freq_hz);
  updateLog(d.log);
}).catch(()=>{});setTimeout(poll,2000);}
function fmtFreq(hz){if(hz>=1e6)return (hz/1e6).toFixed(6)+' MHz';if(hz>=1e3)return (hz/1e3).toFixed(3)+' kHz';return hz+' Hz';}
function setPreset(i){fetch('/api/freq',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({freq_hz:presets[i].f})});}
function setCustom(){const f=parseInt(document.getElementById('custom_freq').value);if(f)fetch('/api/freq',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({freq_hz:f})});}
function setRange(d){range_div10=d;document.getElementById('btn_direct').className=d?'':'active';document.getElementById('btn_div10').className=d?'active':'';}
function setGate(s){gate_s=s;}
function startCount(){fetch('/api/count',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({div10:range_div10,gate:gate_s})});}
function clearLog(){fetch('/api/callog',{method:'DELETE'});}
function updateLog(entries){if(!entries)return;let h='';entries.forEach(e=>{h+=`<div>${e.ts} ${e.inst}: meas=${e.measured} exp=${e.expected} err=${e.error} ${e.units}</div>`;});document.getElementById('log_div').innerHTML=h;}
</script></body></html>
)rawhtml";

// ── Web Handlers ──────────────────────────────────────────────────────────────
void handleRoot() { server.send_P(200, "text/html", HTML_PAGE); }

void handleStatus() {
    char buf[512];
    // Build log JSON
    String log_json = "[";
    int n = min((int)g_cal_log_count, CAL_LOG_ENTRIES);
    for (int i = 0; i < n; i++) {
        int idx = (g_cal_log_idx - n + i + CAL_LOG_ENTRIES) % CAL_LOG_ENTRIES;
        if (i > 0) log_json += ",";
        char entry[128];
        snprintf(entry, sizeof(entry),
                 "{\"ts\":\"%s\",\"inst\":\"%s\",\"measured\":%.3f,\"expected\":%.3f,\"error\":%.3f,\"units\":\"%s\"}",
                 g_cal_log[idx].timestamp, g_cal_log[idx].instrument,
                 g_cal_log[idx].measured_val, g_cal_log[idx].expected_val,
                 g_cal_log[idx].error_val, g_cal_log[idx].units);
        log_json += entry;
    }
    log_json += "]";

    snprintf(buf, sizeof(buf),
             "{\"gps_fix\":%d,\"sats\":%d,\"time\":\"%s\","
             "\"locked\":%d,\"err_ppb\":%.3f,\"correction\":%ld,"
             "\"prog_freq_hz\":%llu,\"rssi_dBm\":%.1f,\"cnt_freq_hz\":%llu,"
             "\"log\":%s}",
             (int)g_gps_fix, g_gps_sats, g_gps_time.c_str(),
             (int)g_disc_locked, g_freq_error_ppb, (long)g_correction,
             g_prog_freq_cHz / 100ULL, g_rssi_cal_dBm, g_cnt_freq_hz,
             log_json.c_str());
    server.send(200, "application/json", buf);
}

void handleSetFreq() {
    if (!server.hasArg("plain")) { server.send(400, "text/plain", "no body"); return; }
    String body = server.arg("plain");
    // parse {"freq_hz": 10000000}
    int idx = body.indexOf("freq_hz");
    if (idx < 0) { server.send(400, "text/plain", "no freq_hz"); return; }
    uint64_t fhz = (uint64_t)body.substring(body.indexOf(':', idx)+1).toInt();
    if (fhz < 8000 || fhz > 160000000) { server.send(400, "text/plain", "out of range"); return; }
    g_prog_freq_cHz = fhz * 100ULL;
    si5351.set_freq(g_prog_freq_cHz, SI5351_CLK2);
    prefs.begin("cal_master", false);
    prefs.putULong64("prog_freq", g_prog_freq_cHz);
    prefs.end();
    server.send(200, "text/plain", "OK");
}

void handleStartCount() {
    if (!server.hasArg("plain")) { server.send(400, "text/plain", "no body"); return; }
    String body = server.arg("plain");
    bool div10 = (body.indexOf("true") >= 0);
    int gate = 1;
    int gi = body.indexOf("gate");
    if (gi >= 0) gate = body.substring(body.indexOf(':', gi)+1).toInt();
    gate = constrain(gate, 1, 10);
    startCounter(div10, gate);
    server.send(200, "text/plain", "counting");
}

void handleCalLog() {
    if (server.method() == HTTP_DELETE) {
        g_cal_log_idx = 0; g_cal_log_count = 0;
        server.send(200, "text/plain", "cleared");
        return;
    }
    // Export as CSV
    String csv = "timestamp,instrument,measured,expected,error,units\r\n";
    int n = min((int)g_cal_log_count, CAL_LOG_ENTRIES);
    for (int i = 0; i < n; i++) {
        int idx = (g_cal_log_idx - n + i + CAL_LOG_ENTRIES) % CAL_LOG_ENTRIES;
        char row[128];
        snprintf(row, sizeof(row), "%s,%s,%.6f,%.6f,%.6f,%s\r\n",
                 g_cal_log[idx].timestamp, g_cal_log[idx].instrument,
                 g_cal_log[idx].measured_val, g_cal_log[idx].expected_val,
                 g_cal_log[idx].error_val, g_cal_log[idx].units);
        csv += row;
    }
    server.send(200, "text/csv", csv);
}

// ── Log Entry Helper ──────────────────────────────────────────────────────────
void logCalEvent(const char *instrument, float measured, float expected, const char *units) {
    CalEntry &e = g_cal_log[g_cal_log_idx];
    snprintf(e.timestamp, sizeof(e.timestamp), "%s", g_gps_time.c_str());
    strncpy(e.instrument, instrument, sizeof(e.instrument)-1);
    e.measured_val = measured;
    e.expected_val = expected;
    e.error_val    = measured - expected;
    strncpy(e.units, units, sizeof(e.units)-1);
    g_cal_log_idx = (g_cal_log_idx + 1) % CAL_LOG_ENTRIES;
    if (g_cal_log_count < CAL_LOG_ENTRIES) g_cal_log_count++;
}

// ── BT Command Handler ────────────────────────────────────────────────────────
void handleBTCmd(const String &cmd) {
    if (cmd.startsWith("FREQ ")) {
        uint64_t f = (uint64_t)cmd.substring(5).toInt();
        if (f >= 8000 && f <= 160000000) {
            g_prog_freq_cHz = f * 100ULL;
            si5351.set_freq(g_prog_freq_cHz, SI5351_CLK2);
            BT.println("OK FREQ");
        }
    } else if (cmd == "STATUS") {
        sendBTPacket();
    } else if (cmd == "COUNT") {
        startCounter(false, COUNTER_GATE_SEC);
        BT.println("OK COUNT");
    } else if (cmd.startsWith("CALLOG ")) {
        // CALLOG instrument measured expected units
        // e.g.: CALLOG NanoVNA 14225000 14225100 Hz
        // (simple manual entry from operator)
        BT.println("OK LOG");
    } else if (cmd == "LOCK?") {
        BT.println(g_disc_locked ? "LOCKED" : "UNLOCKED");
    } else if (cmd == "HELP") {
        BT.println("FREQ <hz> | STATUS | COUNT | LOCK? | HELP");
    }
}

// ── Setup ─────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.println("[CAL] GPS Calibration Station starting");

    pinMode(GPIO_GPS_PPS,   INPUT);
    pinMode(GPIO_RANGE_SEL, OUTPUT);
    pinMode(GPIO_LED_GPS,   OUTPUT);
    pinMode(GPIO_LED_LOCK,  OUTPUT);
    digitalWrite(GPIO_RANGE_SEL, HIGH);
    digitalWrite(GPIO_LED_GPS,   LOW);
    digitalWrite(GPIO_LED_LOCK,  LOW);

    gpsSerial.begin(GPS_UART_BAUD, SERIAL_8N1, GPIO_GPS_TX, GPIO_GPS_RX);
    attachInterrupt(digitalPinToInterrupt(GPIO_GPS_PPS), pps_isr, RISING);

    setupSi5351();
    setupPCNT();

    WiFi.softAP(WIFI_SSID, WIFI_PASS);
    server.on("/",           HTTP_GET,  handleRoot);
    server.on("/api/status", HTTP_GET,  handleStatus);
    server.on("/api/freq",   HTTP_POST, handleSetFreq);
    server.on("/api/count",  HTTP_POST, handleStartCount);
    server.on("/api/callog", handleCalLog);
    server.begin();

    BT.begin(BT_DEVICE_NAME);
    Serial.printf("[CAL] WiFi AP: %s  BT: %s\n", WIFI_SSID, BT_DEVICE_NAME);
}

// ── Loop ──────────────────────────────────────────────────────────────────────
static uint32_t t_last_disc   = 0;
static uint32_t t_last_rssi   = 0;
static uint32_t t_last_bt     = 0;
static String   bt_buf;

void loop() {
    server.handleClient();
    feedGPS();
    updateLEDs();

    // Update counter result when gate closes
    if (!g_cnt_running && g_pcnt_total > 0) {
        updateCounterResult();
    }

    // Discipline loop every (GATE_SECONDS + 1) seconds
    if (millis() - t_last_disc > (GATE_SECONDS + 1) * 1000UL) {
        t_last_disc = millis();
        runDisciplineLoop();
    }

    // RSSI every 500ms
    if (millis() - t_last_rssi > 500) {
        t_last_rssi = millis();
        readRSSI();
    }

    // BT status every 2s
    if (millis() - t_last_bt > 2000) {
        t_last_bt = millis();
        if (BT.connected()) sendBTPacket();
    }

    // BT command receive
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n' || c == '\r') {
            bt_buf.trim();
            if (bt_buf.length() > 0) handleBTCmd(bt_buf);
            bt_buf = "";
        } else {
            bt_buf += c;
        }
    }
}
