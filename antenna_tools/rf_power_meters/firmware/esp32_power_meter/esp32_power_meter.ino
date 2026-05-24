/**
 * esp32_power_meter.ino — TM-PWR-ESP32-001 Rev A
 * ESP32 RF Power Meter Firmware
 * Inline directional (FWD+REF+SWR) and terminating (FWD power) variants
 * Features: peak hold, rolling average, SD logging, BT JSON, USB CSV
 *
 * Libraries (install via Arduino IDE Library Manager):
 *   Adafruit_ADS1X15  >= 2.4.0
 *   Adafruit_SSD1306  >= 2.5.0   (fallback display if no CYD)
 *   Adafruit_GFX      >= 1.11.0
 *   ArduinoJson       >= 6.21.0
 *   DallasTemperature >= 3.9.0    (DS18B20, terminating meter)
 *   OneWire           >= 2.3.7
 *   SD                (built-in ESP32)
 *   BluetoothSerial   (built-in ESP32)
 *   Preferences       (built-in ESP32)
 *   esp_task_wdt      (built-in ESP32)
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Preferences.h>
#include <BluetoothSerial.h>
#include <esp_task_wdt.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ArduinoJson.h>

#ifdef METER_TYPE_TERMINATING
  #include <OneWire.h>
  #include <DallasTemperature.h>
#endif

#include "config.h"
#include "calibration.h"
#include "data_logging.h"

// ─── Display Modes ────────────────────────────────────────────────────────────
enum DisplayMode {
    DISP_LIVE = 0,    // Live FWD, REF, SWR bargraph
    DISP_PEAK,        // Peak hold display
    DISP_AVG,         // Averaging window display
    DISP_LOG,         // Log status (SD, BT, file name)
    DISP_CAL,         // Calibration data view
    DISP_COUNT
};

// ─── Averaging Window Options ─────────────────────────────────────────────────
static const uint32_t AVG_WINDOWS[] = {
    AVG_WINDOW_FAST_MS, AVG_WINDOW_MED_MS, AVG_WINDOW_SLOW_MS
};
static const char* AVG_LABELS[] = {"100ms", "500ms", "2s"};

// ─── Global State ─────────────────────────────────────────────────────────────
Adafruit_ADS1115  ads;
Adafruit_SSD1306  oled(128, 64, &Wire, -1);
BluetoothSerial   btSerial;
Preferences       prefs;
PowerCalibration  pwrCal;
DataLogger        logger;
SweepLogger       sweepLog;

#ifdef METER_TYPE_TERMINATING
  OneWire           ow(DS18B20_PIN);
  DallasTemperature ds18(&ow);
  float             loadTempC = 25.0f;
#endif

PowerReading  current     = {};
PowerReading  peakReading = {};

RollingAvg<50> fwdAvg;    // 50 samples × 100ms interval = 5s max window
RollingAvg<50> refAvg;

float   freqMHz     = 14.0f;
int     rangeIdx    = 0;
bool    autoRange   = true;
int     dispMode    = DISP_LIVE;
int     avgWinIdx   = 1;         // default 500ms
bool    peakHold    = false;
uint32_t peakResetMs= 0;

// ─── Button State ─────────────────────────────────────────────────────────────
struct BtnState { bool last; uint32_t changed; uint32_t held; };
BtnState bPeak  = {true, 0, 0};
BtnState bAvg   = {true, 0, 0};
BtnState bRange = {true, 0, 0};
BtnState bLog   = {true, 0, 0};

// ─── Timing ───────────────────────────────────────────────────────────────────
uint32_t lastMeasMs  = 0;
uint32_t lastOledMs  = 0;
uint32_t lastBtMs    = 0;
uint32_t lastTempMs  = 0;

// ─── ADC Read ─────────────────────────────────────────────────────────────────
float readADC_mV(int ch) {
    int32_t sum = 0;
    for (int i = 0; i < ADC_OVERSAMPLE; i++) {
        sum += ads.readADC_SingleEnded(ch);
        delayMicroseconds(100);
    }
    return (float)sum / ADC_OVERSAMPLE * 0.1875f;  // ADS1115 GAIN_TWOTHIRDS LSB
}

// ─── Auto-Range ───────────────────────────────────────────────────────────────
void autoRangeCheck(float vfwd_raw_mv) {
    if (!autoRange) return;
    if (vfwd_raw_mv > VDET_OVERLOAD_MV && rangeIdx < NUM_RANGES - 1) {
        rangeIdx++;
        digitalWrite(RELAY_1_PIN, rangeIdx >= 1 ? HIGH : LOW);
        digitalWrite(RELAY_2_PIN, rangeIdx >= 2 ? HIGH : LOW);
        delay(RELAY_SETTLE_MS);
    } else if (vfwd_raw_mv < VDET_MIN_MV * 3 && rangeIdx > 0) {
        rangeIdx--;
        digitalWrite(RELAY_1_PIN, rangeIdx >= 1 ? HIGH : LOW);
        digitalWrite(RELAY_2_PIN, rangeIdx >= 2 ? HIGH : LOW);
        delay(RELAY_SETTLE_MS);
    }
}

void applyRange(int idx) {
    rangeIdx = constrain(idx, 0, NUM_RANGES - 1);
    digitalWrite(RELAY_1_PIN, rangeIdx >= 1 ? HIGH : LOW);
    digitalWrite(RELAY_2_PIN, rangeIdx >= 2 ? HIGH : LOW);
    delay(RELAY_SETTLE_MS);
}

// ─── Measurement ─────────────────────────────────────────────────────────────
PowerReading takeMeasurement() {
    PowerReading r = {};
    r.ts_ms  = millis();
    r.range  = rangeIdx;

    float raw_fwd  = readADC_mV(ADC_FWD_CH);
    float raw_ref  = readADC_mV(ADC_REF_CH);
    float raw_peak = readADC_mV(ADC_PEAK_CH);
    float raw_temp = readADC_mV(ADC_TEMP_CH);

    r.vfwd_mv  = pwrCal.applyFwd(raw_fwd);
    r.vref_mv  = pwrCal.applyRef(raw_ref);
    r.vpeak_mv = pwrCal.applyFwd(raw_peak);  // peak uses same cal as fwd

    autoRangeCheck(raw_fwd);

    // Temperature
    r.temp_c = PowerCalibration::ntcTempC(raw_temp);
    float k_temp = PowerCalibration::tempCorrection(r.temp_c);

#ifdef BAND_UHF
    float coupling = PowerCalibration::uhfCouplingDB(freqMHz);
#else
    float coupling = COUPLING_DB;
#endif
    float atten = RANGE_ATTEN_DB[rangeIdx];

    r.fwd_w  = mvToWatts(r.vfwd_mv, coupling, atten) / k_temp;
    r.ref_w  = mvToWatts(r.vref_mv, coupling, atten) / k_temp;
    r.peak_w = mvToWatts(r.vpeak_mv, coupling, atten) / k_temp;

    if (r.vfwd_mv >= VDET_MIN_MV) {
        r.swr    = computeSWR(r.fwd_w, r.ref_w);
        r.gamma  = (r.swr > 1.0f) ? (r.swr - 1.0f) / (r.swr + 1.0f) : 0.0f;
        r.rl_db  = (r.gamma > 0.0001f) ? -20.0f * log10f(r.gamma) : 60.0f;
        r.efficiency = (r.fwd_w > 0.001f)
                       ? (1.0f - r.ref_w / r.fwd_w) * 100.0f : 0.0f;
        r.valid = true;
    }

    // Update rolling averages
    fwdAvg.push(r.fwd_w);
    refAvg.push(r.ref_w);

    // Update peak
    if (r.valid && r.fwd_w > peakReading.fwd_w) peakReading = r;

    return r;
}

// ─── OLED Display ─────────────────────────────────────────────────────────────
void drawOLED(const PowerReading& r) {
    if (millis() - lastOledMs < OLED_UPDATE_MS) return;
    lastOledMs = millis();

    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);

    switch (dispMode) {
    case DISP_LIVE: {
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        if (r.valid)
            oled.printf("SWR %.2f  %.3fMHz", r.swr, freqMHz);
        else
            oled.print("SWR ---  no signal");

        // FWD power (large)
        oled.setTextSize(2);
        oled.setCursor(0, 12);
        if (r.valid) {
            if (r.fwd_w >= 100.0f)     oled.printf("%.0fW", r.fwd_w);
            else if (r.fwd_w >= 10.0f) oled.printf("%.1fW", r.fwd_w);
            else                        oled.printf("%.2fW", r.fwd_w);
        } else {
            oled.print("---");
        }

        oled.setTextSize(1);
        oled.setCursor(0, 34);
        if (r.valid)
            oled.printf("Ref:%.2fW  RL:%.1fdB", r.ref_w, r.rl_db);
        oled.setCursor(0, 44);
        oled.printf("%s  %s", RANGE_LABEL[rangeIdx],
                    autoRange ? "AUTO" : "MAN ");
        oled.setCursor(0, 54);
        oled.printf("%.0fC  %s %s",
            r.temp_c,
            logger.isLogging() ? "LOG" : "   ",
            BAND_STR);
        break;
    }
    case DISP_PEAK: {
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        oled.print("PEAK HOLD");
        oled.setTextSize(2);
        oled.setCursor(0, 12);
        if (peakReading.valid)
            oled.printf("%.2fW", peakReading.fwd_w);
        else
            oled.print("---");
        oled.setTextSize(1);
        oled.setCursor(0, 34);
        if (peakReading.valid) {
            uint32_t ago = (millis() - peakReading.ts_ms) / 1000;
            oled.printf("SWR %.2f  %ds ago", peakReading.swr, ago);
        }
        oled.setCursor(0, 48);
        oled.printf("Live:%.2fW", r.fwd_w);
        break;
    }
    case DISP_AVG: {
        float fwd_a = fwdAvg.avg();
        float ref_a = refAvg.avg();
        float swr_a = computeSWR(fwd_a, ref_a);
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        oled.printf("AVG %s window", AVG_LABELS[avgWinIdx]);
        oled.setTextSize(2);
        oled.setCursor(0, 12);
        oled.printf("%.2fW", fwd_a);
        oled.setTextSize(1);
        oled.setCursor(0, 34);
        oled.printf("SWR %.2f  Ref %.2fW", swr_a, ref_a);
        break;
    }
    case DISP_LOG: {
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        oled.printf("LOG: %s", logger.isLogging() ? "ACTIVE" : "OFF");
        oled.setCursor(0, 12);
        oled.printf("File: %s", logger.getFilename());
        oled.setCursor(0, 22);
        oled.printf("Count: %lu", logger.getCount());
        oled.setCursor(0, 32);
        oled.printf("SD: %s", logger.sd_available ? "OK" : "NONE");
        oled.setCursor(0, 42);
        oled.printf("BT: %s", btSerial.hasClient() ? "CONNECTED" : "---");
        break;
    }
    case DISP_CAL: {
        oled.setTextSize(1);
        oled.setCursor(0, 0);
        oled.print("CALIBRATION");
        oled.setCursor(0, 10);
        oled.printf("FwdZ: %.1fmV", pwrCal.cal.fwd_zero_mv);
        oled.setCursor(0, 20);
        oled.printf("RefZ: %.1fmV", pwrCal.cal.ref_zero_mv);
        oled.setCursor(0, 30);
        oled.printf("FwdS: %.4f", pwrCal.cal.fwd_scale);
        oled.setCursor(0, 40);
        oled.printf("RefS: %.4f", pwrCal.cal.ref_scale);
        oled.setCursor(0, 50);
        oled.printf("Freq: %.3fMHz  %s",
            pwrCal.cal.freq_mhz, pwrCal.cal.valid ? "OK" : "UNCAL");
        break;
    }
    }

    oled.display();
}

// ─── Button Handling ─────────────────────────────────────────────────────────
bool btnPressed(BtnState& b, int pin) {
    bool state = digitalRead(pin);
    if (state != b.last && millis() - b.changed > BTN_DEBOUNCE_MS) {
        b.changed = millis();
        b.last = state;
        if (!state) { b.held = millis(); return true; }
    }
    return false;
}
bool btnHeld(BtnState& b, uint32_t ms) {
    return !b.last && (millis() - b.held > ms);
}

void handleButtons() {
    if (btnPressed(bPeak, BTN_PEAK_PIN)) {
        if (peakHold) {
            peakHold = false;
            peakReading = {};
        } else {
            peakHold = true;
        }
    }
    if (btnHeld(bPeak, 2000)) {
        peakReading = {};
        peakHold    = false;
        // Discharge peak capacitor
        digitalWrite(PEAK_RESET_PIN, HIGH);
        delay(PEAK_RESET_PULSE_MS);
        digitalWrite(PEAK_RESET_PIN, LOW);
    }

    if (btnPressed(bAvg, BTN_AVG_PIN)) {
        avgWinIdx = (avgWinIdx + 1) % 3;
        fwdAvg.reset();
        refAvg.reset();
    }

    if (btnPressed(bRange, BTN_RANGE_PIN)) {
        if (autoRange) {
            autoRange = false;
        } else {
            int next = (rangeIdx + 1) % NUM_RANGES;
            applyRange(next);
        }
    }
    if (btnHeld(bRange, 2000)) {
        autoRange = true;
    }

    if (btnPressed(bLog, BTN_LOG_PIN)) {
        dispMode = (dispMode + 1) % DISP_COUNT;
    }
    if (btnHeld(bLog, 2000)) {
        logger.toggleLogging();
    }
}

// ─── BT / Serial Command Handling ────────────────────────────────────────────
void processCommand(const String& raw) {
    String cmd = raw;
    cmd.trim();

    if (cmd.startsWith("SETFREQ ")) {
        freqMHz = cmd.substring(8).toFloat();
        prefs.begin(NVS_NAMESPACE, false);
        prefs.putFloat(NVS_KEY_FREQ_MHZ, freqMHz);
        prefs.end();
        Serial.printf("FREQ: %.3f MHz\n", freqMHz);
    }
    else if (cmd == "CALZERO") {
        float vf = readADC_mV(ADC_FWD_CH);
        float vr = readADC_mV(ADC_REF_CH);
        pwrCal.calibrateZero(vf, vr);
        prefs.begin(NVS_NAMESPACE, false);
        pwrCal.save(prefs);
        prefs.end();
        Serial.printf("CALZERO: fwd=%.1f ref=%.1f mV\n", vf, vr);
    }
    else if (cmd.startsWith("CALREF ")) {
        float known_w = cmd.substring(7).toFloat();
        if (known_w > 0.0f) {
            float vf = readADC_mV(ADC_FWD_CH);
            float vr = readADC_mV(ADC_REF_CH);
            pwrCal.cal.freq_mhz = freqMHz;
            pwrCal.calibrateScale(vf, known_w, vr);
            prefs.begin(NVS_NAMESPACE, false);
            pwrCal.save(prefs);
            prefs.end();
            Serial.printf("CALREF %.1fW: fwd_scale=%.4f\n",
                          known_w, pwrCal.cal.fwd_scale);
        }
    }
    else if (cmd == "STATUS") {
        logger.sendStatus(btSerial, current, freqMHz);
    }
    else if (cmd == "PEAKRST") {
        peakReading = {};
        peakHold    = false;
        digitalWrite(PEAK_RESET_PIN, HIGH);
        delay(PEAK_RESET_PULSE_MS);
        digitalWrite(PEAK_RESET_PIN, LOW);
        Serial.println("PEAK RESET");
    }
    else if (cmd == "LOGON") {
        if (!logger.sd_available) logger.initSD();
        if (!logger.isLogging()) logger.toggleLogging();
        Serial.println("LOGGING ON");
    }
    else if (cmd == "LOGOFF") {
        if (logger.isLogging()) logger.toggleLogging();
        Serial.println("LOGGING OFF");
    }
    else if (cmd.startsWith("SWEEP ")) {
        // SWEEP start_mhz stop_mhz step_mhz
        float s, e, st;
        sscanf(cmd.c_str() + 6, "%f %f %f", &s, &e, &st);
        sweepLog.start(s, e, st > 0 ? st : 0.1f);
    }
    else if (cmd == "RANGE AUTO") {
        autoRange = true;
        Serial.println("RANGE AUTO");
    }
    else if (cmd.startsWith("RANGE ")) {
        autoRange = false;
        applyRange(cmd.substring(6).toInt() - 1);
        Serial.printf("RANGE %d\n", rangeIdx + 1);
    }
    else if (cmd == "RESET") {
        prefs.begin(NVS_NAMESPACE, false);
        prefs.clear();
        prefs.end();
        Serial.println("NVS cleared — restart");
        delay(500);
        ESP.restart();
    }
    else if (cmd == "INFO") {
        Serial.printf("FW:%s BAND:%s METER:%s CAL:%s\n",
            FW_VERSION, BAND_STR,
#ifdef METER_TYPE_INLINE
            "INLINE",
#else
            "TERM",
#endif
            pwrCal.cal.valid ? "OK" : "NONE");
    }
    else {
        Serial.printf("UNKNOWN: %s\n", cmd.c_str());
    }
}

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.printf("\nRF Power Meter FW %s  Band: %s\n", FW_VERSION, BAND_STR);

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    // ADS1115
    if (!ads.begin(ADS1115_ADDR)) {
        Serial.println("ERROR: ADS1115 not found — check I2C wiring");
    }
    ads.setGain(ADC_PGA);
    ads.setDataRate(ADC_RATE);

    // OLED
    if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("WARN: SSD1306 not found");
    } else {
        oled.clearDisplay();
        oled.setTextColor(SSD1306_WHITE);
        oled.setTextSize(1);
        oled.setCursor(0, 20);
        oled.printf("RF Power Meter\n%s / FW %s", BAND_STR, FW_VERSION);
        oled.display();
        delay(1500);
    }

    // GPIO
    pinMode(RELAY_1_PIN, OUTPUT);  digitalWrite(RELAY_1_PIN, LOW);
    pinMode(RELAY_2_PIN, OUTPUT);  digitalWrite(RELAY_2_PIN, LOW);
    pinMode(PEAK_RESET_PIN, OUTPUT); digitalWrite(PEAK_RESET_PIN, LOW);
    pinMode(BTN_PEAK_PIN,  INPUT_PULLUP);
    pinMode(BTN_AVG_PIN,   INPUT_PULLUP);
    pinMode(BTN_RANGE_PIN, INPUT_PULLUP);
    pinMode(BTN_LOG_PIN,   INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Bluetooth
    btSerial.begin(BT_DEVICE_NAME);

    // Load calibration from NVS
    prefs.begin(NVS_NAMESPACE, true);
    pwrCal.load(prefs);
    freqMHz = prefs.getFloat(NVS_KEY_FREQ_MHZ, 14.0f);
    prefs.end();
    if (!pwrCal.cal.valid)
        Serial.println("WARNING: No calibration. Run CALZERO then CALREF <watts>");

    // DS18B20 (terminating meter)
#ifdef METER_TYPE_TERMINATING
    ds18.begin();
    Serial.printf("DS18B20: %d sensors found\n", ds18.getDeviceCount());
#endif

    // SD card
    logger.initSD();

    // Print CSV header to USB serial
    Serial.print(LOG_CSV_HEADER);

    // Watchdog
    esp_task_wdt_config_t wdt = {WDT_TIMEOUT_S * 1000, 0, true};
    esp_task_wdt_reconfigure(&wdt);
    esp_task_wdt_add(NULL);

    Serial.println("Ready. Commands: STATUS CALZERO CALREF SETFREQ SWEEP LOGON INFO");
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
    esp_task_wdt_reset();

    // Take measurement every 100ms
    if (millis() - lastMeasMs >= 100) {
        lastMeasMs = millis();
        current = takeMeasurement();

        // Sweep mode: auto-advance
        if (sweepLog.active)
            sweepLog.addPoint(sweepLog.current_mhz,
                              current.fwd_w, current.ref_w, current.swr);
    }

    // DS18B20 poll (every 2 seconds)
#ifdef METER_TYPE_TERMINATING
    if (millis() - lastTempMs >= 2000) {
        lastTempMs = millis();
        ds18.requestTemperatures();
        loadTempC = ds18.getTempCByIndex(0);
        if (loadTempC > TEMP_ALERT_C) {
            Serial.printf("THERMAL WARNING: %.1f°C\n", loadTempC);
        }
    }
    current.temp_c = loadTempC;
#endif

    // OLED
    drawOLED(current);

    // Data logging
    logger.logReading(current, freqMHz);

    // BT status JSON (every 500ms if client connected)
    if (btSerial.hasClient() && millis() - lastBtMs >= BT_LOG_INTERVAL_MS) {
        lastBtMs = millis();
        logger.sendStatus(btSerial, current, freqMHz);
    }

    // Buttons
    handleButtons();

    // Serial + BT commands
    String cmd = "";
    if (btSerial.available())    cmd = btSerial.readStringUntil('\n');
    else if (Serial.available()) cmd = Serial.readStringUntil('\n');
    if (cmd.length() > 0)       processCommand(cmd);

    // Status LED
    static uint32_t ledMs = 0;
    static bool     ledOn = false;
    uint32_t blink = current.valid ?
        (current.swr > SWR_WARN ? LED_FAULT_MS :
         current.swr > SWR_GOOD ? LED_WARN_MS  : LED_NORMAL_MS)
        : LED_NORMAL_MS;
    if (millis() - ledMs > blink) {
        ledMs = millis();
        ledOn = !ledOn;
        digitalWrite(LED_PIN, ledOn);
    }

    delay(5);
}
