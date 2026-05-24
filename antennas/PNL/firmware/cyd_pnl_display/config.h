// ============================================================
//  config.h — Pool Noodle Non-Loop Antenna CYD Display
//  TM-PNL2-FW-002 Rev A
//  CYD: ESP32-2432S028R, ILI9341 320×240 TFT, XPT2046 touch
//  Receives Bluetooth serial from PNL-Tuner ESP32
// ============================================================

#pragma once

// ---- Backlight ----
#define TFT_BL_PIN       21
#define BL_BRIGHTNESS    200

// ---- Touch (XPT2046) ----
#define TOUCH_CS         33
#define TOUCH_IRQ        36

// ---- Bluetooth ----
#define BT_TARGET        "PNL-Tuner"
#define BT_TIMEOUT_MS    3000

// ---- Display Pages ----
#define PAGE_SWR         0
#define PAGE_ANTENNA     1   // PNILD coil info / J-pole dimensions / Yagi pattern
#define PAGE_POWER       2
#define PAGE_GUIDE       3
#define NUM_PAGES        4

// ---- Frame Rate ----
#define FPS_INTERVAL_MS  100

// ---- Colors (ILI9341 RGB565) ----
#define C_BG         0x0000
#define C_TEXT        0x07E0
#define C_WARN        0xFFE0
#define C_ALARM       0xF800
#define C_TITLE       0xFFE0
#define C_BORDER      0x2104
#define C_CYAN        0x07FF
#define C_WHITE       0xFFFF
#define C_ORANGE      0xFD20
#define C_DARK_GREEN  0x03E0
#define C_DARK_BLUE   0x000F
#define C_PURPLE      0x780F

// ---- Touch Zone Width ----
#define TOUCH_ZONE_W     80

// ---- BT Packet Data Struct ----
// Packet: "BAND:%s,TYPE:%d,L:%.2f,H:%.1f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,TAP:%d\n"
struct AntData {
    char    band[8];
    int     ant_type;      // 0=PNILD, 1=J-pole, 2=Yagi
    float   L_uH;          // loading coil (PNILD)
    float   h_arm_m;       // arm length (PNILD)
    float   swr;
    float   fwd_watts;
    float   eff_pct;
    int     state;         // 0=IDLE, 1=TUNING, 2=LOCKED, 3=PASSTHRU
    int     relay_tap;     // active relay (0–8; 255=none)
    bool    valid;
    unsigned long last_update_ms;
};
