// ============================================================
//  config.h — Pool Noodle Magnetic Loop CYD Display
//  TM-PNL-FW-002 Rev A
//  CYD: ESP32-2432S028R, ILI9341 320x240 TFT, XPT2046 touch
//  Receives Bluetooth serial from PNML-Tuner ESP32
// ============================================================

#pragma once

// ---- Backlight ----
#define TFT_BL_PIN       21
#define BL_BRIGHTNESS    200    // 0-255

// ---- Touch (XPT2046) ----
#define TOUCH_CS         33
#define TOUCH_IRQ        36

// ---- Bluetooth ----
#define BT_TARGET        "PNML-Tuner"
#define BT_TIMEOUT_MS    3000   // ms without packet before "connecting" screen

// ---- Display Pages ----
#define PAGE_SWR         0
#define PAGE_CAP         1
#define PAGE_POWER       2
#define PAGE_GUIDE       3
#define NUM_PAGES        4

// ---- Frame Rate ----
#define FPS_INTERVAL_MS  100    // 10 FPS

// ---- ILI9341 RGB565 Color Palette ----
#define C_BG             0x0000   // black
#define C_TEXT           0x07E0   // green
#define C_WARN           0xFFE0   // yellow
#define C_ALARM          0xF800   // red
#define C_TITLE          0xFFE0   // yellow
#define C_BORDER         0x2104   // dark grey
#define C_CYAN           0x07FF   // cyan
#define C_WHITE          0xFFFF   // white
#define C_ORANGE         0xFD20   // orange
#define C_DARK_RED       0x8000   // dark red (HV warning bg)
#define C_DARK_GREEN     0x03E0   // dim green
#define C_DARK_BLUE      0x000F   // dim blue

// ---- Touch Zone Width ----
#define TOUCH_ZONE_W     80      // pixels; left/right zones for page nav

// ---- MAX_CAP_STEPS (must match config.h in esp32_loop_tuner) ----
#define MAX_CAP_STEPS_CYD 327680UL

// ---- Data struct populated from BT packet ----
// Packet format: "BAND:%s,POS:%ld,C:%.0f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,HOMED:%d\n"
struct LoopData {
    char    band[8];
    float   freq_mhz;
    long    cap_pos;
    float   C_pF;
    float   swr;
    float   fwd_watts;
    float   eff_pct;
    int     state;       // 0=IDLE 1=HOMING 2=TUNING 3=LOCKED 4=QUAD_RDY
    bool    homed;
    bool    valid;       // true if SWR reading is valid (RF present)
    bool    is_quad;     // true if current band is resonant quad (C_pF==0 and state==4)
    unsigned long last_update_ms;
};
