#pragma once
/**
 * menu.h — TM-CWND-ESP32-001 Rev A
 * OLED menu system, preset storage, SD card logging
 */

#include <Preferences.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "winder.h"

// ─── Menu Items ───────────────────────────────────────────────────────────────

enum MenuID {
    // Top level
    MENU_WIND = 0,
    MENU_SETTINGS,
    MENU_PRESETS,
    MENU_CALC,
    MENU_STATUS,
    MENU_HOME_TRAV,

    // Wind sub-menu
    MENU_WIND_SET_TURNS,
    MENU_WIND_SET_L,
    MENU_WIND_SET_FORMER,
    MENU_WIND_SET_AWG,
    MENU_WIND_SET_PITCH,
    MENU_WIND_SET_LAYERS,
    MENU_WIND_START,

    // Settings sub-menu
    MENU_SET_RPM,
    MENU_SET_DIRECTION,
    MENU_SET_TRAV_HOME,

    // Calculator sub-menu
    MENU_CALC_SINGLE,
    MENU_CALC_MULTI,
    MENU_CALC_RESONANT,
    MENU_CALC_WIRE_LEN,

    // Presets sub-menu
    MENU_PRESET_LOAD,
    MENU_PRESET_SAVE,
    MENU_PRESET_LIST,

    MENU_COUNT
};

struct MenuItem {
    MenuID   id;
    MenuID   parent;
    const char* label;
};

static const MenuItem MENU_ITEMS[] = {
    {MENU_WIND,           MENU_COUNT,  "WIND"},
    {MENU_SETTINGS,       MENU_COUNT,  "SETTINGS"},
    {MENU_PRESETS,        MENU_COUNT,  "PRESETS"},
    {MENU_CALC,           MENU_COUNT,  "CALCULATOR"},
    {MENU_STATUS,         MENU_COUNT,  "STATUS"},
    {MENU_HOME_TRAV,      MENU_COUNT,  "HOME TRAVERSE"},

    {MENU_WIND_SET_TURNS, MENU_WIND,   "Set Turns"},
    {MENU_WIND_SET_L,     MENU_WIND,   "Set Target L"},
    {MENU_WIND_SET_FORMER,MENU_WIND,   "Set Former Dia"},
    {MENU_WIND_SET_AWG,   MENU_WIND,   "Set Wire AWG"},
    {MENU_WIND_SET_PITCH, MENU_WIND,   "Set Pitch"},
    {MENU_WIND_SET_LAYERS,MENU_WIND,   "Set Layers"},
    {MENU_WIND_START,     MENU_WIND,   "> START WIND"},

    {MENU_SET_RPM,        MENU_SETTINGS,"Set RPM"},
    {MENU_SET_DIRECTION,  MENU_SETTINGS,"Direction"},
    {MENU_SET_TRAV_HOME,  MENU_SETTINGS,"Home Traverse"},

    {MENU_CALC_SINGLE,    MENU_CALC,   "Single Layer L"},
    {MENU_CALC_MULTI,     MENU_CALC,   "Multi Layer L"},
    {MENU_CALC_RESONANT,  MENU_CALC,   "Resonant Turns"},
    {MENU_CALC_WIRE_LEN,  MENU_CALC,   "Wire Length"},

    {MENU_PRESET_LOAD,    MENU_PRESETS,"Load Preset"},
    {MENU_PRESET_SAVE,    MENU_PRESETS,"Save Preset"},
    {MENU_PRESET_LIST,    MENU_PRESETS,"List Presets"},
};
static const int N_MENU_ITEMS = sizeof(MENU_ITEMS) / sizeof(MENU_ITEMS[0]);

// ─── Preset Storage ───────────────────────────────────────────────────────────

#define MAX_PRESETS   10
#define PRESET_KEY_PREFIX "prs"

class PresetManager {
public:
    Preferences* prefs = nullptr;

    void begin(Preferences* p) { prefs = p; }

    bool save(int slot, const WindingProgram& prog) {
        if (slot < 0 || slot >= MAX_PRESETS || !prefs) return false;
        char key[16];
        snprintf(key, sizeof(key), "%s%d", PRESET_KEY_PREFIX, slot);
        // Serialize to compact struct
        prefs->putBytes(key, &prog, sizeof(WindingProgram));
        return true;
    }

    bool load(int slot, WindingProgram& prog) {
        if (slot < 0 || slot >= MAX_PRESETS || !prefs) return false;
        char key[16];
        snprintf(key, sizeof(key), "%s%d", PRESET_KEY_PREFIX, slot);
        if (prefs->getBytesLength(key) == 0) return false;
        prefs->getBytes(key, &prog, sizeof(WindingProgram));
        prog.calculate();
        return true;
    }

    int countUsed() {
        int n = 0;
        for (int i = 0; i < MAX_PRESETS; i++) {
            char key[16];
            snprintf(key, sizeof(key), "%s%d", PRESET_KEY_PREFIX, i);
            if (prefs->getBytesLength(key) > 0) n++;
        }
        return n;
    }

    bool getPresetName(int slot, char* name, int len) {
        WindingProgram p;
        if (!load(slot, p)) return false;
        strncpy(name, p.name, len);
        return true;
    }
};

// ─── SD Card Logger ───────────────────────────────────────────────────────────

class SDLogger {
public:
    bool available = false;
    char log_path[32] = "/coil_log.csv";

    bool begin() {
        if (!SD.begin(PIN_SD_CS)) {
            Serial.println("SD: not found or init failed");
            available = false;
            return false;
        }
        // Write header if new file
        if (!SD.exists(log_path)) {
            File f = SD.open(log_path, FILE_WRITE);
            if (f) {
                f.println("date,time,name,awg,former_mm,turns,layers,"
                          "calc_uh,wire_m,resistance_ohm,pitch");
                f.close();
            }
        }
        available = true;
        Serial.printf("SD: logging to %s\n", log_path);
        return true;
    }

    void logWinding(const WindingProgram& p) {
        if (!available) return;
        File f = SD.open(log_path, FILE_APPEND);
        if (!f) return;
        // Date/time from millis (no RTC — use session counter)
        unsigned long now = millis() / 1000;
        f.printf("sess,%lu,%s,%d,%.1f,%lu,%d,%.2f,%.2f,%.3f,%.2f\n",
            now, p.name, p.awg, p.former_dia_mm, (unsigned long)p.target_turns,
            p.layers, p.calc_uh, p.total_wire_m, p.dc_resistance, p.pitch_factor);
        f.close();
        Serial.printf("SD: logged '%s' (%.1f µH, %lu turns)\n",
            p.name, p.calc_uh, (unsigned long)p.target_turns);
    }

    bool listEntries(char* buf, int buf_len) {
        if (!available) { strncpy(buf, "SD not ready", buf_len); return false; }
        File f = SD.open(log_path);
        if (!f) return false;
        int n = 0;
        snprintf(buf, buf_len, "SD log: ");
        while (f.available() && n < buf_len - 2) {
            buf[n++] = f.read();
        }
        buf[n] = 0;
        f.close();
        return true;
    }
};

// ─── Rotary Encoder ───────────────────────────────────────────────────────────

class RotaryEncoder {
public:
    volatile int  delta   = 0;
    volatile bool pressed = false;
    int8_t last_a = 1, last_b = 1;

    static RotaryEncoder* instance;
    static void IRAM_ATTR isrA() {
        int a = digitalRead(PIN_ENC_A);
        int b = digitalRead(PIN_ENC_B);
        if (instance->last_a == 1 && a == 0) {
            instance->delta += (b == 1) ? +1 : -1;
        }
        instance->last_a = a;
    }
    static void IRAM_ATTR isrBtn() {
        instance->pressed = true;
    }

    void begin() {
        instance = this;
        pinMode(PIN_ENC_A, INPUT_PULLUP);
        pinMode(PIN_ENC_B, INPUT_PULLUP);
        pinMode(PIN_ENC_BTN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), isrA, CHANGE);
        attachInterrupt(digitalPinToInterrupt(PIN_ENC_BTN), isrBtn, FALLING);
    }

    int getDelta() {
        noInterrupts(); int d = delta; delta = 0; interrupts(); return d;
    }

    bool getPressed() {
        noInterrupts(); bool p = pressed; pressed = false; interrupts(); return p;
    }
};
RotaryEncoder* RotaryEncoder::instance = nullptr;

// ─── OLED Menu System ─────────────────────────────────────────────────────────

class MenuSystem {
public:
    Adafruit_SSD1306* oled    = nullptr;
    WinderController* winder  = nullptr;
    PresetManager*    presets = nullptr;
    SDLogger*         logger  = nullptr;
    RotaryEncoder*    enc     = nullptr;

    MenuID   current_menu  = MENU_COUNT;  // top level
    int      cursor        = 0;
    int      scroll_offset = 0;
    bool     in_edit       = false;
    float    edit_val      = 0;
    int      edit_int      = 0;
    int      active_preset = 0;
    char     status_msg[64] = "";

    void begin(Adafruit_SSD1306* d, WinderController* w,
               PresetManager* p, SDLogger* l, RotaryEncoder* e) {
        oled = d; winder = w; presets = p; logger = l; enc = e;
    }

    // Build list of children for current_menu
    int buildChildList(MenuID* out, int max_out) {
        int n = 0;
        if (current_menu == MENU_COUNT) {
            // Top level: collect items without parent (parent == MENU_COUNT)
            for (int i = 0; i < N_MENU_ITEMS && n < max_out; i++)
                if (MENU_ITEMS[i].parent == MENU_COUNT)
                    out[n++] = MENU_ITEMS[i].id;
        } else {
            for (int i = 0; i < N_MENU_ITEMS && n < max_out; i++)
                if (MENU_ITEMS[i].parent == current_menu)
                    out[n++] = MENU_ITEMS[i].id;
        }
        return n;
    }

    void update() {
        int d = enc->getDelta();
        bool btn = enc->getPressed();

        if (in_edit) {
            edit_val += d;
            if (btn) { applyEdit(); in_edit = false; }
        } else {
            MenuID children[16];
            int n = buildChildList(children, 16);
            cursor = constrain(cursor + d, 0, n - 1);
            if (d != 0) scroll_offset = max(0, cursor - 2);
            if (btn && n > 0) navigate(children[cursor]);
        }

        if (winder->state == WINDER_WINDING ||
            winder->state == WINDER_DONE ||
            winder->state == WINDER_FAULT)
            drawWindingStatus();
        else
            drawMenu();
    }

    void navigate(MenuID id) {
        switch (id) {
            // Wind program parameters — enter edit mode
            case MENU_WIND_SET_TURNS:
                in_edit = true;
                edit_val = winder->program.target_turns;
                break;
            case MENU_WIND_SET_FORMER:
                in_edit = true;
                edit_val = winder->program.former_dia_mm;
                break;
            case MENU_WIND_SET_AWG:
                in_edit = true;
                edit_val = winder->program.awg;
                break;
            case MENU_WIND_SET_PITCH:
                in_edit = true;
                edit_val = winder->program.pitch_factor * 10;
                break;
            case MENU_WIND_SET_LAYERS:
                in_edit = true;
                edit_val = winder->program.layers;
                break;
            case MENU_WIND_SET_L:
                in_edit = true;
                edit_val = winder->program.target_uh;
                break;

            case MENU_WIND_START:
                winder->startWinding();
                snprintf(status_msg, sizeof(status_msg), "Winding...");
                break;

            case MENU_SET_RPM:
                in_edit = true;
                edit_val = DEFAULT_RPM;
                break;

            case MENU_HOME_TRAV:
            case MENU_SET_TRAV_HOME:
                winder->startHoming();
                snprintf(status_msg, sizeof(status_msg), "Homing traverse...");
                break;

            case MENU_PRESET_SAVE:
                if (presets) {
                    presets->save(active_preset, winder->program);
                    if (logger) logger->logWinding(winder->program);
                    snprintf(status_msg, sizeof(status_msg),
                        "Saved to slot %d", active_preset);
                }
                break;
            case MENU_PRESET_LOAD:
                if (presets) presets->load(active_preset, winder->program);
                snprintf(status_msg, sizeof(status_msg),
                    "Loaded slot %d", active_preset);
                break;
            case MENU_PRESET_LIST:
                in_edit = true;
                edit_val = active_preset;
                break;

            case MENU_CALC_SINGLE: drawCalcResult(); break;
            case MENU_CALC_RESONANT: drawResonantCalc(); break;

            default:
                // Navigate into sub-menu
                current_menu = id;
                cursor = 0;
                scroll_offset = 0;
                break;
        }
    }

    void applyEdit() {
        MenuID children[16];
        int n = buildChildList(children, 16);
        if (cursor >= n) return;
        MenuID editing = children[cursor];
        switch (editing) {
            case MENU_WIND_SET_TURNS:
                winder->program.target_turns = (uint32_t)max(1.0f, edit_val);
                winder->program.target_uh = 0;  // turns-mode
                break;
            case MENU_WIND_SET_FORMER:
                winder->program.former_dia_mm = max(6.35f, edit_val);
                break;
            case MENU_WIND_SET_AWG:
                winder->program.awg = (uint8_t)constrain(edit_val, 10, 30);
                break;
            case MENU_WIND_SET_PITCH:
                winder->program.pitch_factor = max(1.0f, edit_val / 10.0f);
                break;
            case MENU_WIND_SET_LAYERS:
                winder->program.layers = (uint8_t)constrain(edit_val, 1, MAX_LAYERS);
                break;
            case MENU_WIND_SET_L:
                winder->program.target_uh = max(0.0f, edit_val);
                if (winder->program.target_uh > 0 &&
                    winder->program.former_dia_mm > 0 &&
                    winder->program.coil_len_mm > 0)
                    winder->program.target_turns = WinderMath::turnsForInductance(
                        winder->program.target_uh,
                        winder->program.former_dia_mm,
                        winder->program.coil_len_mm);
                break;
            case MENU_SET_RPM:
                winder->setSpeed(max(MIN_RPM, edit_val));
                break;
            case MENU_PRESET_LIST:
                active_preset = (int)constrain(edit_val, 0, MAX_PRESETS - 1);
                break;
            default: break;
        }
        winder->program.calculate();
    }

    void drawMenu() {
        if (!oled) return;
        oled->clearDisplay();
        oled->setTextSize(1);
        oled->setTextColor(SSD1306_WHITE);

        // Title bar
        oled->setCursor(0, 0);
        if (current_menu == MENU_COUNT) oled->print("COIL WINDER");
        else {
            for (int i = 0; i < N_MENU_ITEMS; i++)
                if (MENU_ITEMS[i].id == current_menu)
                    oled->print(MENU_ITEMS[i].label);
        }

        if (in_edit) {
            oled->setCursor(0, 16);
            oled->printf("Edit: %.1f", edit_val);
            oled->setCursor(0, 28);
            oled->print("Turn=adjust  Push=OK");
            oled->display();
            return;
        }

        MenuID children[16];
        int n = buildChildList(children, 16);
        int visible = min(n - scroll_offset, 4);
        for (int i = 0; i < visible; i++) {
            int idx = i + scroll_offset;
            oled->setCursor(8, 12 + i * 12);
            if (idx == cursor) { oled->fillRect(0, 11 + i * 12, 128, 12, SSD1306_WHITE); oled->setTextColor(SSD1306_BLACK); }
            else oled->setTextColor(SSD1306_WHITE);
            for (int j = 0; j < N_MENU_ITEMS; j++)
                if (MENU_ITEMS[j].id == children[idx])
                    oled->print(MENU_ITEMS[j].label);
            oled->setTextColor(SSD1306_WHITE);
        }

        // Status line
        if (status_msg[0]) {
            oled->setCursor(0, 56);
            oled->print(status_msg);
        }
        oled->display();
    }

    void drawWindingStatus() {
        if (!oled) return;
        oled->clearDisplay();
        oled->setTextSize(1);
        oled->setTextColor(SSD1306_WHITE);

        const char* state_str[] = {"IDLE","WINDING","PAUSED","HOMING","FAULT","DONE"};
        oled->setCursor(0, 0);
        oled->printf("%-7s  AWG%d  L1",
            state_str[(int)winder->state], winder->program.awg);

        // Large turn count
        oled->setTextSize(2);
        oled->setCursor(0, 14);
        char buf[16];
        snprintf(buf, sizeof(buf), "%lu/%lu",
            (unsigned long)winder->turns.turns,
            (unsigned long)winder->program.target_turns);
        oled->print(buf);

        oled->setTextSize(1);
        oled->setCursor(0, 36);
        oled->printf("%.1f RPM  L%.0fuH",
            winder->turns.rpm, winder->program.calc_uh);

        oled->setCursor(0, 48);
        oled->printf("Lay:%d  Pos:%.1fmm",
            winder->traverse.layer + 1, winder->traverse.pos_mm);

        if (winder->isFault()) {
            const char* fault_str[] = {"OK","WIRE BREAK","OVERRUN","TRAV LIM","E-STOP"};
            oled->setCursor(0, 56);
            oled->printf("FAULT: %s", fault_str[(int)winder->fault]);
        }
        oled->display();
    }

    void drawCalcResult() {
        WindingProgram p = winder->program;
        p.calculate();
        float L = WinderMath::calcInductance(p);
        char buf[80];
        snprintf(buf, sizeof(buf),
            "D=%.0fmm N=%lu L=%.1fuH\nQ@7MHz=%.0f",
            p.former_dia_mm, (unsigned long)p.target_turns, L,
            WinderMath::qFactor(7.0f, L, p.awg, p.total_wire_m));
        Serial.println(buf);
        snprintf(status_msg, sizeof(status_msg), "L=%.2f uH", L);
    }

    void drawResonantCalc() {
        float L = winder->program.target_uh > 0 ?
            winder->program.target_uh : WinderMath::calcInductance(winder->program);
        float f_srf = WinderMath::selfResonantFreq(L, 5.0f);  // 5pF stray
        snprintf(status_msg, sizeof(status_msg),
            "SRF~%.0fMHz", f_srf);
    }
};
