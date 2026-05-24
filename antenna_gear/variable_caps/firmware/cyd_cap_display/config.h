// ============================================================
//  config.h — Variable Capacitor CYD Display
//  TM-VCAP-CYD-001 Rev A
//  ESP32-2432S028R (CYD): ILI9341 320×240 TFT, XPT2046 touch
//  Bluetooth SPP master → "RF-CapTuner" ESP32 controller
// ============================================================

#pragma once

// ---- Bluetooth ----
#define BT_TARGET_NAME      "RF-CapTuner"
#define BT_RETRY_MS         5000

// ---- Display pages ----
#define PAGE_LIVE    0
#define PAGE_BAND    1
#define PAGE_SWEEP   2
#define PAGE_INFO    3
#define N_PAGES      4

// ---- Frame rate ----
#define DISPLAY_UPDATE_MS   500    // full redraw interval (LIVE uses partial update)
#define PARTIAL_UPDATE_MS   200    // partial update interval for LIVE page

// ---- Touch ----
#define TOUCH_DEBOUNCE_MS   300
#define XPT_RAW_MIN         200
#define XPT_RAW_MAX         3900

// ---- Voltage rating thresholds ----
// Ratio of estimated Vpeak / V_rated_kV → color
#define VOLT_SAFE_RATIO     0.50f   // < 50% rated → green
#define VOLT_WARN_RATIO     0.80f   // 50–80% rated → orange
// > 80% rated → red (danger)

// ---- Sparkline history ----
#define SPARK_N             60      // 60 points (~30s at 2 Hz)

// ---- Sweep buffer ----
#define SWEEP_PTS           200     // max sweep data points

// ---- Stale data timeout ----
#define STALE_TIMEOUT_MS    3000    // show STALE if no BT packet for 3s

// ---- Capacitor design voltage ratings (kV peak) ----
// Indexed by CapDesignType from tuner: QRP=0, HF100=1, TX=2, VHF=3, UHF=4
static const float CAP_V_RATING_KV[] = {
    0.200f,   // VC-QRP
    1.500f,   // VC-HF100
    4.000f,   // VC-TX
    0.500f,   // VC-VHF
    0.200f,   // VC-UHF
};

// ---- CYD pin definitions (fixed by hardware) ----
#define TFT_BL_PIN          21
#define BL_BRIGHTNESS       200     // 0-255 PWM
#define TOUCH_CS            33
#define TOUCH_IRQ           36
#define RGB_R               4
#define RGB_G               16
#define RGB_B               17
#define CYD_LDR             34      // light sensor (optional)
