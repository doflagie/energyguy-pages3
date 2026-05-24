// ============================================================
//  cyd_loop_display.ino — Pool Noodle Magnetic Loop Display
//  TM-PNL-FW-002 Rev A
//  CYD (ESP32-2432S028), ILI9341 320x240, TFT_eSPI library
//  Bluetooth serial from PNML-Tuner; 4-page touch UI at 10 FPS
// ============================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <BluetoothSerial.h>
#include "config.h"

// ---- Hardware objects ----
TFT_eSPI      tft;
TFT_eSprite   spr(&tft);
BluetoothSerial BT;

// ---- State ----
LoopData      g_data    = {};
int           g_page    = PAGE_SWR;
bool          g_bt_conn = false;
String        g_bt_buf  = "";
uint32_t      g_last_draw = 0;
uint32_t      g_last_touch = 0;

// ---- Helper: SWR color ----
uint16_t swrColor(float swr) {
    if (swr < 1.5f) return C_TEXT;
    if (swr < 2.5f) return C_WARN;
    return C_ALARM;
}

// ---- Helper: state name ----
const char* stateName(int s) {
    switch (s) {
        case 0: return "IDLE";
        case 1: return "HOMING";
        case 2: return "TUNING";
        case 3: return "LOCKED";
        case 4: return "QUAD RDY";
        default: return "?";
    }
}

// ---- Backlight ----
void setupBacklight() {
    ledcSetup(0, 5000, 8);
    ledcAttachPin(TFT_BL_PIN, 0);
    ledcWrite(0, BL_BRIGHTNESS);
}

// ---- BT Packet Parser ----
bool parseBTPacket(const String& line) {
    // Format: "BAND:%s,POS:%ld,C:%.0f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,HOMED:%d"
    char band[8] = {};
    long pos = 0;
    float C = 0, swr = 0, fwd = 0, eff = 0;
    int state = 0, homed = 0;

    int n = sscanf(line.c_str(),
        "BAND:%7[^,],POS:%ld,C:%f,SWR:%f,FWD:%fW,EFF:%f,STATE:%d,HOMED:%d",
        band, &pos, &C, &swr, &fwd, &eff, &state, &homed);

    if (n < 6) return false;

    strncpy(g_data.band, band, sizeof(g_data.band) - 1);
    g_data.cap_pos  = pos;
    g_data.C_pF     = C;
    g_data.swr      = swr;
    g_data.fwd_watts = fwd;
    g_data.eff_pct  = eff;
    g_data.state    = state;
    g_data.homed    = (homed != 0);
    g_data.valid    = (swr > 0.5f && swr < 10.5f);
    g_data.is_quad  = (C == 0.0f && state == 4);
    g_data.last_update_ms = millis();
    return true;
}

// ============================================================
//  PAGE 0 — SWR METER
// ============================================================
void drawPage_SWR() {
    spr.fillSprite(C_BG);

    // Title bar
    spr.setTextColor(C_TITLE, C_BG);
    spr.setTextSize(2);
    spr.setCursor(4, 4);
    spr.print(g_data.band);
    spr.setTextSize(1);
    spr.setTextColor(C_WHITE, C_BG);
    spr.setCursor(80, 8);
    char tmp[32];
    snprintf(tmp, sizeof(tmp), "%.3f MHz", g_data.freq_mhz);
    spr.print(tmp);

    // State label top-right
    spr.setTextColor(C_CYAN, C_BG);
    spr.setCursor(240, 4);
    spr.print(stateName(g_data.state));

    // Loop type
    spr.setTextColor(g_data.is_quad ? C_CYAN : C_TEXT, C_BG);
    spr.setCursor(4, 24);
    spr.print(g_data.is_quad ? "RESONANT QUAD" : "SMALL TX LOOP");

    if (!g_data.valid) {
        spr.setTextColor(C_WARN, C_BG);
        spr.setTextSize(2);
        spr.setCursor(20, 90);
        spr.print("NO RF DETECTED");
        spr.setTextSize(1);
        spr.setCursor(20, 120);
        spr.print("Transmit briefly to measure SWR");
    } else {
        // Big SWR number
        uint16_t sc = swrColor(g_data.swr);
        spr.setTextColor(sc, C_BG);
        spr.setTextSize(4);
        snprintf(tmp, sizeof(tmp), "%.2f", g_data.swr);
        spr.setCursor(30, 55);
        spr.print(tmp);
        spr.setTextSize(2);
        spr.setCursor(190, 65);
        spr.print(": 1");
        spr.setTextSize(1);
        spr.setTextColor(C_WHITE, C_BG);
        spr.setCursor(4, 100);
        spr.print("SWR");

        // SWR bar
        int bar_x = 4, bar_y = 115, bar_w = 312, bar_h = 16;
        spr.drawRect(bar_x, bar_y, bar_w, bar_h, C_BORDER);
        float swr_norm = constrain((g_data.swr - 1.0f) / 9.0f, 0.0f, 1.0f);
        int filled = (int)(swr_norm * (bar_w - 2));
        uint16_t bar_col = (g_data.swr < 1.5f) ? C_TEXT : (g_data.swr < 2.5f) ? C_WARN : C_ALARM;
        spr.fillRect(bar_x + 1, bar_y + 1, filled, bar_h - 2, bar_col);
        spr.setTextColor(C_BORDER, C_BG);
        for (int s = 2; s <= 10; s += 2) {
            int x = bar_x + (int)((s - 1.0f) / 9.0f * (bar_w - 2));
            spr.drawFastVLine(x, bar_y, bar_h, C_BORDER);
        }
        // Scale labels
        spr.setTextColor(C_WHITE, C_BG);
        spr.setCursor(bar_x, bar_y + bar_h + 2);     spr.print("1");
        spr.setCursor(bar_x + bar_w/2 - 4, bar_y + bar_h + 2); spr.print("5");
        spr.setCursor(bar_x + bar_w - 10, bar_y + bar_h + 2); spr.print("10");

        // Return loss
        spr.setCursor(4, 148);
        spr.setTextColor(C_CYAN, C_BG);
        snprintf(tmp, sizeof(tmp), "Return Loss: %.1f dB", g_data.valid ? -20.0f * log10f(g_data.swr > 1 ? (g_data.swr-1)/(g_data.swr+1) : 0.001f) : 0.0f);
        spr.print(tmp);
    }

    // Nav hint
    spr.setTextColor(C_BORDER, C_BG);
    spr.setCursor(4, 230);   spr.print("<");
    spr.setCursor(308, 230); spr.print(">");

    spr.pushSprite(0, 0);
}

// ============================================================
//  PAGE 1 — CAPACITOR / QUAD INFO
// ============================================================
void drawPage_CAP() {
    spr.fillSprite(C_BG);
    spr.setTextColor(C_TITLE, C_BG);
    spr.setTextSize(2);
    spr.setCursor(4, 4);

    if (g_data.is_quad) {
        spr.print("QUAD LOOP");
        spr.setTextColor(C_CYAN, C_BG);
        spr.setTextSize(1);
        spr.setCursor(4, 28);
        spr.print("RESONANT LOOP — No capacitor required");
        spr.setCursor(4, 50);
        spr.setTextColor(C_WHITE, C_BG);
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "Efficiency: %.1f%%", g_data.eff_pct);
        spr.print(tmp);
        spr.setCursor(4, 65);
        spr.print("Feed: 2.5:1 UNUN or gamma match");
        spr.setCursor(4, 80);
        spr.print("Zin approx 125 Ohm (free space quad)");
        spr.setCursor(4, 100);
        spr.setTextColor(C_TEXT, C_BG);
        spr.print("Tune: adjust gamma rod for SWR min");
    } else {
        spr.print("CAPACITOR");
        spr.setTextSize(1);
        spr.setTextColor(C_WHITE, C_BG);
        char tmp[64];

        // Position
        float pos_pct = (float)g_data.cap_pos / MAX_CAP_STEPS_CYD * 100.0f;
        snprintf(tmp, sizeof(tmp), "Position: %ld steps (%.1f%%)", g_data.cap_pos, pos_pct);
        spr.setCursor(4, 30); spr.print(tmp);

        // Estimated C — large
        spr.setTextColor(C_CYAN, C_BG);
        spr.setTextSize(3);
        snprintf(tmp, sizeof(tmp), "%.0f pF", g_data.C_pF);
        spr.setCursor(20, 50); spr.print(tmp);
        spr.setTextSize(1);
        spr.setTextColor(C_BORDER, C_BG);
        spr.setCursor(20, 82); spr.print("estimated C");

        // Arc gauge showing position
        int cx = 270, cy = 60, r = 48;
        spr.drawCircle(cx, cy, r, C_BORDER);
        float angle_rad = pos_pct / 100.0f * 3.14159f * 1.5f - 3.14159f * 0.75f;
        int ax = cx + (int)(r * cosf(angle_rad));
        int ay = cy + (int)(r * sinf(angle_rad));
        spr.drawLine(cx, cy, ax, ay, C_CYAN);
        spr.drawCircle(cx, cy, 4, C_CYAN);

        // Q and efficiency
        spr.setTextColor(C_WHITE, C_BG);
        spr.setCursor(4, 100);
        snprintf(tmp, sizeof(tmp), "Efficiency: %.1f%%   Q: (band est)", g_data.eff_pct);
        spr.print(tmp);

        // HV Warning for high-power low bands
        bool hv = (g_data.C_pF > 30.0f) || (g_data.band[0] == '1' || (g_data.band[0] == '8'));
        if (hv) {
            spr.fillRect(0, 120, 320, 90, C_DARK_RED);
            spr.setTextColor(C_WHITE, C_DARK_RED);
            spr.setTextSize(2);
            spr.setCursor(8, 128);
            spr.print("!! HIGH VOLTAGE !!");
            spr.setTextSize(1);
            spr.setCursor(8, 152);
            spr.print("Lethal cap voltage present during TX.");
            spr.setCursor(8, 164);
            spr.print("Bleeder 100kOhm MUST be installed.");
            spr.setCursor(8, 176);
            spr.print("Discharge before touching cap area.");
        }

        if (!g_data.homed) {
            spr.fillRect(0, 215, 320, 20, C_WARN);
            spr.setTextColor(C_BG, C_WARN);
            spr.setCursor(4, 219);
            spr.print("NOT HOMED — run /home before tuning");
        }
    }

    spr.setTextColor(C_BORDER, C_BG);
    spr.setCursor(4, 230);   spr.print("<");
    spr.setCursor(308, 230); spr.print(">");
    spr.pushSprite(0, 0);
}

// ============================================================
//  PAGE 2 — POWER
// ============================================================
void drawPage_POWER() {
    spr.fillSprite(C_BG);
    spr.setTextColor(C_TITLE, C_BG);
    spr.setTextSize(2);
    spr.setCursor(4, 4);
    spr.print("POWER");

    spr.setTextSize(1);
    if (!g_data.valid) {
        spr.setTextColor(C_WARN, C_BG);
        spr.setCursor(4, 50);
        spr.print("No RF detected");
    } else {
        char tmp[48];
        // Big forward power
        spr.setTextSize(3);
        spr.setTextColor(C_TEXT, C_BG);
        if (g_data.fwd_watts < 1.0f) {
            snprintf(tmp, sizeof(tmp), "%.0f mW", g_data.fwd_watts * 1000);
        } else {
            snprintf(tmp, sizeof(tmp), "%.2f W", g_data.fwd_watts);
        }
        spr.setCursor(4, 35);
        spr.print(tmp);
        spr.setTextSize(1);
        spr.setTextColor(C_WHITE, C_BG);

        float fwd_dbm = 10.0f * log10f(g_data.fwd_watts * 1000.0f);
        snprintf(tmp, sizeof(tmp), "Fwd: %.1f dBm", fwd_dbm);
        spr.setCursor(4, 80); spr.print(tmp);

        float refl_w = g_data.fwd_watts * powf((g_data.swr - 1.0f)/(g_data.swr + 1.0f), 2.0f);
        float refl_dbm = refl_w > 0 ? 10.0f * log10f(refl_w * 1000.0f) : -99.0f;
        snprintf(tmp, sizeof(tmp), "Refl: %.1f dBm", refl_dbm);
        spr.setCursor(4, 95); spr.print(tmp);

        float rl = -20.0f * log10f(constrain((g_data.swr - 1.0f)/(g_data.swr + 1.0f), 0.001f, 1.0f));
        snprintf(tmp, sizeof(tmp), "Return loss: %.1f dB", rl);
        spr.setCursor(4, 110); spr.print(tmp);

        snprintf(tmp, sizeof(tmp), "Ant efficiency: %.1f%%", g_data.eff_pct);
        spr.setCursor(4, 125); spr.setTextColor(C_CYAN, C_BG); spr.print(tmp);

        // Power bar
        int bar_x=4, bar_y=145, bar_w=312, bar_h=16;
        float bar_norm = constrain(g_data.fwd_watts / 100.0f, 0.0f, 1.0f);
        int filled = (int)(bar_norm * (bar_w - 2));
        spr.drawRect(bar_x, bar_y, bar_w, bar_h, C_BORDER);
        spr.fillRect(bar_x+1, bar_y+1, filled, bar_h-2,
                     g_data.fwd_watts < 10 ? C_TEXT : g_data.fwd_watts < 50 ? C_WARN : C_ALARM);
        spr.setTextColor(C_WHITE, C_BG);
        spr.setCursor(bar_x, bar_y+bar_h+2);      spr.print("0W");
        spr.setCursor(bar_x+bar_w/2-8, bar_y+bar_h+2); spr.print("50W");
        spr.setCursor(bar_x+bar_w-18, bar_y+bar_h+2);  spr.print("100W");
    }

    spr.setTextColor(C_BORDER, C_BG);
    spr.setCursor(4, 230);   spr.print("<");
    spr.setCursor(308, 230); spr.print(">");
    spr.pushSprite(0, 0);
}

// ============================================================
//  PAGE 3 — GUIDE
// ============================================================
void drawPage_GUIDE() {
    spr.fillSprite(C_BG);
    spr.setTextColor(C_TITLE, C_BG);
    spr.setTextSize(2);
    spr.setCursor(4, 4);
    spr.print(g_data.band);
    spr.setTextSize(1);
    spr.setTextColor(g_data.is_quad ? C_CYAN : C_TEXT, C_BG);
    spr.setCursor(80, 10);
    spr.print(g_data.is_quad ? "RESONANT QUAD" : "SMALL TX LOOP");

    spr.setTextColor(C_WHITE, C_BG);
    int y = 30;
    if (g_data.is_quad) {
        const char* steps[] = {
            "1. Install correct quad for band",
            "2. Apply low-power RF",
            "3. Slide gamma rod for SWR min",
            "4. Adjust gamma cap for < 1.5:1",
            "5. Re-verify at rated power",
        };
        for (int i = 0; i < 5; i++) {
            spr.setCursor(4, y); spr.print(steps[i]); y += 14;
        }
    } else {
        const char* steps[] = {
            "1. HOME cap if not homed",
            "2. Press TUNE or /tune endpoint",
            "3. Verify SWR < 1.5:1 at dip",
            "4. If min SWR > 1.5: adjust coupling loop",
            "5. Re-tune when QSYing > 10 kHz",
        };
        for (int i = 0; i < 5; i++) {
            spr.setCursor(4, y); spr.print(steps[i]); y += 14;
        }
    }

    y += 6;
    spr.setTextColor(C_BORDER, C_BG);
    spr.drawFastHLine(0, y, 320, C_BORDER);
    y += 4;

    spr.setTextColor(C_CYAN, C_BG);
    char tmp[48];
    snprintf(tmp, sizeof(tmp), "Efficiency: %.1f%%", g_data.eff_pct);
    spr.setCursor(4, y); spr.print(tmp); y += 14;

    if (!g_data.is_quad) {
        snprintf(tmp, sizeof(tmp), "Q est: ---  BW est: --- kHz");
        spr.setCursor(4, y); spr.setTextColor(C_WHITE, C_BG); spr.print(tmp); y += 14;
    }

    snprintf(tmp, sizeof(tmp), "State: %s  Homed: %s", stateName(g_data.state), g_data.homed ? "YES" : "NO");
    spr.setTextColor(C_WARN, C_BG);
    spr.setCursor(4, y); spr.print(tmp);

    spr.setTextColor(C_BORDER, C_BG);
    spr.setCursor(4, 230);   spr.print("<");
    spr.setCursor(308, 230); spr.print(">");
    spr.pushSprite(0, 0);
}

// ============================================================
//  Connecting Screen
// ============================================================
void drawConnecting() {
    static uint32_t last_blink = 0;
    static int      dots = 0;
    if (millis() - last_blink > 500) { last_blink = millis(); dots = (dots + 1) % 4; }
    spr.fillSprite(C_BG);
    spr.setTextColor(C_WARN, C_BG);
    spr.setTextSize(2);
    spr.setCursor(20, 100);
    spr.print("BT: CONNECTING");
    for (int d = 0; d < dots; d++) spr.print(".");
    spr.setTextSize(1);
    spr.setTextColor(C_WHITE, C_BG);
    spr.setCursor(20, 130);
    spr.print(BT_TARGET);
    spr.pushSprite(0, 0);
}

// ============================================================
//  Touch Handling
// ============================================================
void handleTouch() {
    if (millis() - g_last_touch < 300) return;
    uint16_t tx, ty;
    // Simple resistive touch read (XPT2046 via SPI)
    // Use TFT_eSPI's built-in getTouch if available
    if (tft.getTouch(&tx, &ty, 40)) {
        g_last_touch = millis();
        if (tx < TOUCH_ZONE_W) {
            g_page = (g_page - 1 + NUM_PAGES) % NUM_PAGES;
        } else if (tx > 320 - TOUCH_ZONE_W) {
            g_page = (g_page + 1) % NUM_PAGES;
        }
    }
}

// ============================================================
//  Setup
// ============================================================
void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);      // landscape, USB connector right
    tft.fillScreen(C_BG);
    setupBacklight();
    spr.createSprite(320, 240);

    tft.setTextColor(C_WARN, C_BG);
    tft.setTextSize(2);
    tft.setCursor(20, 110);
    tft.print("Connecting to BT...");
    tft.setCursor(20, 135);
    tft.setTextSize(1);
    tft.print(BT_TARGET);

    BT.begin("PNML-Display", true);   // client mode
    bool connected = BT.connect(BT_TARGET);
    g_bt_conn = connected;

    g_data.last_update_ms = 0;
    Serial.printf("BT connect: %s\n", connected ? "OK" : "FAIL");
}

// ============================================================
//  Loop
// ============================================================
void loop() {
    // Read BT serial
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n') {
            if (g_bt_buf.length() > 5) parseBTPacket(g_bt_buf);
            g_bt_buf = "";
        } else {
            g_bt_buf += c;
        }
    }

    // Reconnect if dropped
    static uint32_t last_reconnect = 0;
    if (!BT.connected() && millis() - last_reconnect > 3000) {
        last_reconnect = millis();
        g_bt_conn = BT.connect(BT_TARGET);
    }

    handleTouch();

    // Draw at 10 FPS
    if (millis() - g_last_draw >= FPS_INTERVAL_MS) {
        g_last_draw = millis();
        bool no_data = (millis() - g_data.last_update_ms > BT_TIMEOUT_MS || g_data.last_update_ms == 0);

        if (no_data) {
            drawConnecting();
        } else {
            switch (g_page) {
                case PAGE_SWR:   drawPage_SWR();   break;
                case PAGE_CAP:   drawPage_CAP();   break;
                case PAGE_POWER: drawPage_POWER(); break;
                case PAGE_GUIDE: drawPage_GUIDE(); break;
            }
        }
    }
}
