// ============================================================
//  cyd_pnl_display.ino — Pool Noodle Non-Loop Antenna Display
//  TM-PNL2-FW-002 Rev A
//  CYD ESP32-2432S028R, ILI9341 320×240, XPT2046 touch
//  4 pages: SWR, Antenna Info, Power, Operating Guide
// ============================================================

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "BluetoothSerial.h"
#include "config.h"

TFT_eSPI       tft;
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
BluetoothSerial BT;

AntData g_ant;
int     g_page = PAGE_SWR;
bool    g_dirty = true;

// ============================================================
// BT Packet Parser
// "BAND:%s,TYPE:%d,L:%.2f,H:%.1f,SWR:%.2f,FWD:%.3fW,EFF:%.1f,STATE:%d,TAP:%d\n"
// ============================================================
bool parseBTPacket(const String& pkt) {
    AntData d;
    char band_buf[8] = {0};
    int parsed = sscanf(pkt.c_str(),
        "BAND:%7[^,],TYPE:%d,L:%f,H:%f,SWR:%f,FWD:%fW,EFF:%f,STATE:%d,TAP:%d",
        band_buf, &d.ant_type, &d.L_uH, &d.h_arm_m,
        &d.swr, &d.fwd_watts, &d.eff_pct, &d.state, &d.relay_tap);
    if (parsed < 9) return false;
    strncpy(d.band, band_buf, 7);
    d.valid = true;
    d.last_update_ms = millis();
    g_ant = d;
    return true;
}

// ============================================================
// Connecting Screen
// ============================================================
void drawConnecting() {
    static int dot_phase = 0;
    static unsigned long last_dot = 0;
    tft.fillScreen(C_BG);
    tft.setTextColor(C_CYAN, C_BG);
    tft.setTextSize(2);
    tft.drawString("PNL ANTENNA TUNER", 20, 30);
    tft.setTextSize(1);
    tft.setTextColor(C_TEXT, C_BG);
    tft.drawString("Connecting to PNL-Tuner", 20, 80);
    tft.drawString("via Bluetooth Serial...", 20, 96);
    // Blinking dots
    if (millis() - last_dot > 500) {
        dot_phase = (dot_phase + 1) % 4;
        last_dot = millis();
    }
    String dots = "";
    for (int i = 0; i < dot_phase; i++) dots += ".";
    tft.fillRect(0, 120, 320, 20, C_BG);
    tft.drawString("Waiting" + dots, 20, 120);

    tft.setTextColor(0x3186, C_BG);  // dim grey
    tft.drawString("PNL-Tuner WiFi: PNL-Tuner / antenna73", 8, 200);
    tft.drawString("Web: 192.168.4.1", 8, 216);
}

// ============================================================
// Top status bar (common to all pages)
// ============================================================
void drawTopBar() {
    tft.fillRect(0, 0, 320, 22, 0x1082);
    tft.setTextColor(C_TITLE, 0x1082);
    tft.setTextSize(2);
    tft.drawString(g_ant.band, 4, 2);

    // Antenna type tag
    const char* type_str = (g_ant.ant_type == 0) ? "PNILD" :
                           (g_ant.ant_type == 1) ? "J-POLE" : "YAGI";
    tft.setTextColor(C_CYAN, 0x1082);
    tft.drawString(type_str, 90, 4);

    // State tag
    const char* states[] = {"IDLE","TUNING","LOCKED","PASS"};
    uint16_t sc = (g_ant.state == 2) ? C_DARK_GREEN :
                  (g_ant.state == 1) ? C_WARN : C_TEXT;
    tft.setTextColor(sc, 0x1082);
    tft.drawString(states[g_ant.state], 200, 4);

    // Page indicator dots
    for (int p = 0; p < NUM_PAGES; p++) {
        uint16_t dot_c = (p == g_page) ? C_WHITE : C_BORDER;
        tft.fillCircle(276 + p * 12, 11, 4, dot_c);
    }
}

// ============================================================
// PAGE 0: SWR Meter
// ============================================================
void drawPageSWR() {
    tft.fillRect(0, 22, 320, 218, C_BG);
    drawTopBar();

    // Large SWR number
    float swr_disp = g_ant.valid ? g_ant.swr : 0.0f;
    tft.setTextColor(swr_disp < 1.5f ? C_DARK_GREEN :
                     swr_disp < 2.5f ? C_WARN : C_ALARM, C_BG);
    tft.setTextSize(4);
    char swr_buf[10];
    if (g_ant.valid)
        snprintf(swr_buf, sizeof(swr_buf), "%.2f", swr_disp);
    else
        snprintf(swr_buf, sizeof(swr_buf), "  ---");
    tft.drawString("SWR", 10, 38);
    tft.drawString(swr_buf, 90, 38);

    // SWR bar graph (1.0 to 10.0)
    int bar_x = 10, bar_y = 100, bar_w = 300, bar_h = 28;
    tft.drawRect(bar_x - 1, bar_y - 1, bar_w + 2, bar_h + 2, C_BORDER);
    if (g_ant.valid && swr_disp > 1.0f) {
        float fill = fminf((swr_disp - 1.0f) / 9.0f, 1.0f);
        int fill_px = (int)(fill * bar_w);
        uint16_t bar_c = (swr_disp < 1.5f) ? C_DARK_GREEN :
                         (swr_disp < 2.5f) ? C_WARN : C_ALARM;
        tft.fillRect(bar_x, bar_y, fill_px, bar_h, bar_c);
        tft.fillRect(bar_x + fill_px, bar_y, bar_w - fill_px, bar_h, C_BG);
    } else {
        tft.fillRect(bar_x, bar_y, bar_w, bar_h, C_BG);
    }
    tft.setTextSize(1);
    tft.setTextColor(C_BORDER, C_BG);
    tft.drawString("1.0", 8, 133);
    tft.drawString("2.0", 86, 133);
    tft.drawString("3.0", 163, 133);
    tft.drawString("5.0", 241, 133);
    tft.drawString("10.0", 296, 133);

    // Forward power
    tft.setTextSize(2);
    tft.setTextColor(C_TEXT, C_BG);
    char pwr_buf[24];
    if (g_ant.valid)
        snprintf(pwr_buf, sizeof(pwr_buf), "FWD: %.3f W", g_ant.fwd_watts);
    else
        snprintf(pwr_buf, sizeof(pwr_buf), "FWD:  --- W");
    tft.drawString(pwr_buf, 10, 155);

    // Efficiency
    tft.setTextColor(C_CYAN, C_BG);
    char eff_buf[24];
    snprintf(eff_buf, sizeof(eff_buf), "Eff: %.1f%%", g_ant.eff_pct);
    tft.drawString(eff_buf, 10, 178);

    // Touch zone hints
    tft.setTextColor(C_BORDER, C_BG);
    tft.setTextSize(1);
    tft.drawString("[TAP L/R TO CHANGE PAGE]", 55, 228);
}

// ============================================================
// PAGE 1: Antenna Info
// ============================================================
void drawPageAntenna() {
    tft.fillRect(0, 22, 320, 218, C_BG);
    drawTopBar();
    tft.setTextSize(1);
    int y = 30;

    if (g_ant.ant_type == 0) {
        // PNILD
        tft.setTextColor(C_TITLE, C_BG);
        tft.drawString("CENTER-LOADED SHORTENED DIPOLE", 10, y); y += 14;

        char buf[48];
        tft.setTextColor(C_CYAN, C_BG);
        snprintf(buf, sizeof(buf), "L_load:  %.2f uH", g_ant.L_uH);
        tft.drawString(buf, 10, y); y += 14;
        snprintf(buf, sizeof(buf), "Arm:     %.1f m  (%.1f m total)", g_ant.h_arm_m, g_ant.h_arm_m*2);
        tft.drawString(buf, 10, y); y += 14;
        snprintf(buf, sizeof(buf), "Eff:     %.1f%%", g_ant.eff_pct);
        tft.setTextColor(g_ant.eff_pct > 75.0f ? C_DARK_GREEN : C_WARN, C_BG);
        tft.drawString(buf, 10, y); y += 14;

        // Relay tap indicator
        tft.setTextColor(C_ORANGE, C_BG);
        snprintf(buf, sizeof(buf), "Relay tap: %d (of 9)", g_ant.relay_tap);
        tft.drawString(buf, 10, y); y += 14;

        // Visual relay row
        tft.setTextColor(C_BORDER, C_BG);
        tft.drawString("Tap:  0  1  2  3  4  5  6  7  8", 10, y); y += 12;
        tft.drawString("Band: 160 80 40 30 20 17 15 12 10", 10, y); y += 14;
        if (g_ant.relay_tap < 9) {
            int rx = 42 + g_ant.relay_tap * 20;
            tft.fillRect(rx, y - 27, 16, 8, C_ORANGE);
        }

        tft.setTextColor(C_TEXT, C_BG);
        tft.drawString("1:1 current choke balun required", 10, y + 5); y += 18;
        tft.drawString("Balun: 8T coax on FT-240-61", 10, y);

    } else if (g_ant.ant_type == 1) {
        // J-Pole
        tft.setTextColor(C_TITLE, C_BG);
        tft.drawString("J-POLE ANTENNA", 10, y); y += 14;
        tft.setTextColor(C_CYAN, C_BG);
        tft.drawString("Lambda/2 radiator + lambda/4 stub", 10, y); y += 14;
        tft.setTextColor(C_TEXT, C_BG);
        tft.drawString("No tuning required. SWR monitor only.", 10, y); y += 14;
        tft.drawString("Adjust feed tap for SWR minimum.", 10, y); y += 14;
        tft.setTextColor(C_DARK_GREEN, C_BG);
        tft.drawString("Gain: ~2.15 dBi (same as dipole)", 10, y); y += 14;
        tft.setTextColor(C_CYAN, C_BG);
        tft.drawString("Pattern: Omnidirectional", 10, y); y += 14;
        tft.setTextColor(C_WARN, C_BG);
        tft.drawString("Use ferrite choke at feed point!", 10, y);

    } else {
        // Yagi
        tft.setTextColor(C_TITLE, C_BG);
        tft.drawString("YAGI-UDA ANTENNA", 10, y); y += 14;
        tft.setTextColor(C_CYAN, C_BG);
        tft.drawString("Directional gain antenna", 10, y); y += 14;
        tft.setTextColor(C_TEXT, C_BG);
        tft.drawString("No tuning required. SWR monitor only.", 10, y); y += 14;
        tft.setTextColor(C_DARK_GREEN, C_BG);
        tft.drawString("Point antenna for max signal.", 10, y); y += 14;
        tft.setTextColor(C_CYAN, C_BG);
        tft.drawString("F/B ratio: 20-28 dB", 10, y); y += 14;
        tft.setTextColor(C_WARN, C_BG);
        tft.drawString("Hairpin match: adjust for SWR<1.5", 10, y); y += 14;
        tft.setTextColor(C_TEXT, C_BG);
        tft.drawString("Element accuracy: ±0.5-1mm required", 10, y);
    }

    tft.setTextColor(C_BORDER, C_BG);
    tft.drawString("[TAP L/R TO CHANGE PAGE]", 55, 228);
}

// ============================================================
// PAGE 2: Power
// ============================================================
void drawPagePower() {
    tft.fillRect(0, 22, 320, 218, C_BG);
    drawTopBar();
    tft.setTextSize(2);

    char buf[32];
    snprintf(buf, sizeof(buf), "SWR:  %.2f", g_ant.swr);
    tft.setTextColor(g_ant.swr < 1.5f ? C_DARK_GREEN : C_WARN, C_BG);
    tft.drawString(buf, 10, 36);

    snprintf(buf, sizeof(buf), "FWD:  %.3f W", g_ant.fwd_watts);
    tft.setTextColor(C_TEXT, C_BG);
    tft.drawString(buf, 10, 64);

    float refl_pct = (g_ant.fwd_watts > 0.001f) ?
        (g_ant.fwd_watts - g_ant.fwd_watts / (g_ant.swr * g_ant.swr)) * 100.0f / g_ant.fwd_watts : 0.0f;
    snprintf(buf, sizeof(buf), "Eff:  %.1f%%", g_ant.eff_pct);
    tft.setTextColor(g_ant.eff_pct > 75.0f ? C_DARK_GREEN :
                     g_ant.eff_pct > 50.0f ? C_WARN : C_ALARM, C_BG);
    tft.drawString(buf, 10, 92);

    tft.setTextSize(1);
    tft.setTextColor(C_CYAN, C_BG);
    tft.drawString("Antenna efficiency is computed from", 10, 130);
    tft.drawString("coil Q and loading inductance.", 10, 143);
    tft.drawString("Actual depends on coil quality.", 10, 156);

    tft.setTextColor(C_BORDER, C_BG);
    tft.drawString("[TAP L/R TO CHANGE PAGE]", 55, 228);
}

// ============================================================
// PAGE 3: Operating Guide
// ============================================================
void drawPageGuide() {
    tft.fillRect(0, 22, 320, 218, C_BG);
    drawTopBar();
    tft.setTextSize(1);
    tft.setTextColor(C_TITLE, C_BG);
    tft.drawString("OPERATING GUIDE", 10, 30);

    int y = 46;
    if (g_ant.ant_type == 0) {
        tft.setTextColor(C_TEXT, C_BG);
        const char* lines[] = {
            "1. Install coil on noodle form.",
            "2. Connect arm wires to coil ends.",
            "3. Install 1:1 balun at feed point.",
            "4. Connect 50O coax to balun.",
            "5. Press TUNE: relay scan for SWR min.",
            "6. If SWR>2:1: check coil connections.",
            "7. Retune after moving > 5 kHz.",
            "BAND NOTES:",
        };
        for (const char* l : lines) { tft.drawString(l, 10, y); y += 13; }
        tft.setTextColor(C_WARN, C_BG);
        tft.drawString("160/80m: low eff; CW/FT8 preferred.", 10, y);
    } else if (g_ant.ant_type == 1) {
        tft.setTextColor(C_TEXT, C_BG);
        const char* lines[] = {
            "1. Mount pool noodle base on mast.",
            "2. Stub wire runs in noodle groove.",
            "3. Short stub at base (both wires).",
            "4. Radiator wire above noodle; gap=25mm.",
            "5. Feed coax at tap (start: 5% of stub).",
            "6. Adjust tap for SWR minimum < 1.5:1.",
            "7. Install ferrite choke at feed.",
            "8. Raise antenna as high as possible.",
        };
        for (const char* l : lines) { tft.drawString(l, 10, y); y += 13; }
    } else {
        tft.setTextColor(C_TEXT, C_BG);
        const char* lines[] = {
            "1. Cut all elements to ±0.5mm.",
            "2. Clip elements perpendicular to boom.",
            "3. Spacing from reflector: see CSV table.",
            "4. Mount hairpin match at driven element.",
            "5. Connect coax center/shield to split DE.",
            "6. Install choke balun at coax junction.",
            "7. Aim boom at target (max signal direction).",
        };
        for (const char* l : lines) { tft.drawString(l, 10, y); y += 13; }
    }

    tft.setTextColor(C_BORDER, C_BG);
    tft.drawString("[TAP L/R TO CHANGE PAGE]", 55, 228);
}

// ============================================================
// Touch Handler
// ============================================================
void handleTouch() {
    if (!touch.touched()) return;
    static unsigned long last_touch = 0;
    if (millis() - last_touch < 300) return;
    last_touch = millis();

    TS_Point p = touch.getPoint();
    int x = map(p.x, 200, 3800, 0, 320);

    if (x < TOUCH_ZONE_W) {
        g_page = (g_page + NUM_PAGES - 1) % NUM_PAGES;
        g_dirty = true;
    } else if (x > 320 - TOUCH_ZONE_W) {
        g_page = (g_page + 1) % NUM_PAGES;
        g_dirty = true;
    }
}

// ============================================================
// Setup & Loop
// ============================================================
void setup() {
    Serial.begin(115200);

    analogWrite(TFT_BL_PIN, BL_BRIGHTNESS);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(C_BG);

    touch.begin();

    BT.begin("CYD-PNL", true);   // master mode
    BT.connect(BT_TARGET);

    memset(&g_ant, 0, sizeof(g_ant));
    strncpy(g_ant.band, "---", 7);
    g_ant.ant_type = 0;
    g_ant.state    = 0;
    g_ant.relay_tap= 0;
}

void loop() {
    handleTouch();

    // BT receive
    static String pkt_buf = "";
    while (BT.available()) {
        char c = BT.read();
        if (c == '\n') {
            if (parseBTPacket(pkt_buf)) g_dirty = true;
            pkt_buf = "";
        } else {
            pkt_buf += c;
            if (pkt_buf.length() > 200) pkt_buf = "";
        }
    }

    // BT reconnect if timeout
    if (millis() - g_ant.last_update_ms > BT_TIMEOUT_MS) {
        drawConnecting();
        if (!BT.connected()) BT.connect(BT_TARGET);
        return;
    }

    // Redraw on dirty flag or periodic refresh
    static unsigned long last_draw = 0;
    if (g_dirty || millis() - last_draw > 500) {
        g_dirty = false;
        last_draw = millis();
        switch (g_page) {
            case PAGE_SWR:      drawPageSWR();      break;
            case PAGE_ANTENNA:  drawPageAntenna();  break;
            case PAGE_POWER:    drawPagePower();    break;
            case PAGE_GUIDE:    drawPageGuide();    break;
        }
    }
}
