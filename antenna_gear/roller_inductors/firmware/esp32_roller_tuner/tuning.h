/**
 * tuning.h — TM-ROLLER-ESP32-001 Rev A
 * Automatic Inductance Tuning Algorithms and Preset Management
 */

#pragma once
#include <Arduino.h>
#include "config.h"
#include "position.h"

// ─── Band Presets ─────────────────────────────────────────────────────────────

struct BandPreset {
    const char* name;
    float       freq_mhz;
    float       l_uh;          // Typical inductance for this band
    float       l_low_uh;      // Inductance range low (band edges)
    float       l_high_uh;     // Inductance range high
};

static const BandPreset BAND_PRESETS[] = {
    { "160m",  1.9f,  28.0f, 22.0f, 34.0f },
    { "80m",   3.7f,  18.0f, 14.0f, 25.0f },
    { "60m",   5.3f,  12.0f,  9.0f, 16.0f },
    { "40m",   7.1f,   8.0f,  6.0f, 12.0f },
    { "30m",  10.1f,   5.5f,  4.0f,  8.0f },
    { "20m",  14.2f,   3.5f,  2.5f,  5.5f },
    { "17m",  18.1f,   2.2f,  1.5f,  3.5f },
    { "15m",  21.2f,   1.5f,  1.0f,  2.5f },
    { "12m",  24.9f,   1.1f,  0.8f,  1.8f },
    { "10m",  28.5f,   0.8f,  0.5f,  1.5f },
};
static const int N_BAND_PRESETS = sizeof(BAND_PRESETS) / sizeof(BandPreset);


// ─── User Saved Positions ─────────────────────────────────────────────────────

struct SavedPosition {
    float    l_uh;
    long     steps;
    char     label[16];
    bool     valid;
};

static const int MAX_SAVED = 10;


// ─── Auto-Tune Controller ─────────────────────────────────────────────────────

class AutoTuner {
public:
    PositionManager* pos;
    SavedPosition    saved[MAX_SAVED];
    int              n_saved = 0;

    // Sweep state
    bool   sweep_active   = false;
    float  sweep_l_start  = L_MIN_UH;
    float  sweep_l_stop   = L_MAX_UH;
    float  sweep_l_step   = 0.5f;
    float  sweep_l_current = 0.0f;

    void begin(PositionManager* p) { pos = p; }

    // Go to nearest band preset for given frequency
    void goToBandPreset(float freq_mhz) {
        const BandPreset* best = nullptr;
        float min_dist = 9999.0f;
        for (int i = 0; i < N_BAND_PRESETS; i++) {
            float d = fabsf(BAND_PRESETS[i].freq_mhz - freq_mhz);
            if (d < min_dist) { min_dist = d; best = &BAND_PRESETS[i]; }
        }
        if (best) {
            Serial.printf("PRESET: %s → %.1f µH\n", best->name, best->l_uh);
            pos->moveToL(best->l_uh);
        }
    }

    // Go to band preset by name (e.g., "40m")
    bool goToNamedBand(const char* name) {
        for (int i = 0; i < N_BAND_PRESETS; i++) {
            if (strcasecmp(BAND_PRESETS[i].name, name) == 0) {
                pos->moveToL(BAND_PRESETS[i].l_uh);
                return true;
            }
        }
        return false;
    }

    // Save current position with label
    bool savePosition(const char* label) {
        if (n_saved >= MAX_SAVED) return false;
        if (!pos->isHomed()) return false;
        saved[n_saved].l_uh  = pos->currentL();
        saved[n_saved].steps = pos->position_steps;
        strncpy(saved[n_saved].label, label, 15);
        saved[n_saved].valid = true;
        n_saved++;
        Serial.printf("SAVED[%d]: %s = %.2f µH\n", n_saved-1, label, pos->currentL());
        return true;
    }

    // Recall saved position by index
    bool recallPosition(int idx) {
        if (idx < 0 || idx >= n_saved || !saved[idx].valid) return false;
        pos->moveTo(saved[idx].steps);
        Serial.printf("RECALL[%d]: %s → %.2f µH\n", idx, saved[idx].label, saved[idx].l_uh);
        return true;
    }

    // Recall by label
    bool recallByLabel(const char* label) {
        for (int i = 0; i < n_saved; i++) {
            if (saved[i].valid && strcasecmp(saved[i].label, label) == 0)
                return recallPosition(i);
        }
        return false;
    }

    // Start inductance sweep for SWR optimization
    void startSweep(float l_start, float l_stop, float l_step) {
        if (!pos->isHomed()) { Serial.println("ERR: not homed"); return; }
        sweep_l_start   = l_start;
        sweep_l_stop    = l_stop;
        sweep_l_step    = l_step;
        sweep_l_current = l_start;
        sweep_active    = true;
        pos->moveToL(l_start);
        Serial.printf("SWEEP: %.1f–%.1f µH step %.2f µH\n", l_start, l_stop, l_step);
    }

    // Call from main loop; advances sweep when motor is stopped
    void updateSweep() {
        if (!sweep_active) return;
        if (pos->isMoving()) return;

        // Arrived at current sweep point — emit CSV data
        Serial.printf("SWEEP_PT,%.3f,%.3f\n",
            sweep_l_current, pos->currentL());

        sweep_l_current += sweep_l_step;
        if (sweep_l_current > sweep_l_stop) {
            sweep_active = false;
            Serial.println("SWEEP_END");
            return;
        }
        pos->moveToL(sweep_l_current);
    }

    // Build inductance lookup table (prints to serial as CSV)
    void printLookupTable() {
        Serial.println("turns,L_calc_uh,pos_mm,steps");
        for (float n = 1.0f; n <= COIL_TURNS_TOTAL; n += 0.5f) {
            float l  = pos->calc.calcL(n);
            float mm = pos->calc.turnsToMM(n);
            long  s  = pos->calc.mmToSteps(mm);
            Serial.printf("%.1f,%.3f,%.2f,%ld\n", n, l, mm, s);
        }
        Serial.println("TABLE_END");
    }

    // Print list of saved positions
    void printSaved() {
        Serial.printf("SAVED: %d positions\n", n_saved);
        for (int i = 0; i < n_saved; i++) {
            if (saved[i].valid)
                Serial.printf("  [%d] %s = %.2f µH (step=%ld)\n",
                    i, saved[i].label, saved[i].l_uh, saved[i].steps);
        }
    }

    // Print band preset list
    void printBandPresets() {
        Serial.println("BAND_PRESETS:");
        for (int i = 0; i < N_BAND_PRESETS; i++) {
            Serial.printf("  %-5s  %5.1f MHz  L=%.1f µH  [%.1f–%.1f µH]\n",
                BAND_PRESETS[i].name,
                BAND_PRESETS[i].freq_mhz,
                BAND_PRESETS[i].l_uh,
                BAND_PRESETS[i].l_low_uh,
                BAND_PRESETS[i].l_high_uh);
        }
    }
};


// ─── Command Parser ───────────────────────────────────────────────────────────

class CommandParser {
public:
    PositionManager* pos;
    AutoTuner*       tuner;

    void begin(PositionManager* p, AutoTuner* t) { pos = p; tuner = t; }

    // Parse and execute a command string
    // Returns response string
    String process(const String& cmd_raw) {
        String cmd = cmd_raw;
        cmd.trim();
        cmd.toUpperCase();

        if (cmd == "HOME") {
            pos->startHoming();
            return "OK: homing started";
        }
        if (cmd == "STATUS") {
            return buildStatus();
        }
        if (cmd.startsWith("GOTO ") || cmd.startsWith("SETL ")) {
            float val = cmd.substring(5).toFloat();
            pos->moveToL(val);
            return String("OK: moving to ") + val + " uH";
        }
        if (cmd.startsWith("GOSTEP ")) {
            long steps = cmd.substring(7).toInt();
            pos->moveTo(steps);
            return String("OK: moving to step ") + steps;
        }
        if (cmd.startsWith("STEP ")) {
            long delta = cmd.substring(5).toInt();
            pos->moveRelative(delta);
            return String("OK: relative move ") + delta + " steps";
        }
        if (cmd.startsWith("BAND ")) {
            String band = cmd.substring(5);
            band.toLowerCase();
            bool ok = tuner->goToNamedBand(band.c_str());
            return ok ? "OK: going to band preset" : "ERR: unknown band";
        }
        if (cmd.startsWith("FREQ ")) {
            float f = cmd.substring(5).toFloat();
            tuner->goToBandPreset(f);
            return String("OK: preset for ") + f + " MHz";
        }
        if (cmd.startsWith("SAVE ")) {
            String label = cmd.substring(5);
            bool ok = tuner->savePosition(label.c_str());
            return ok ? "OK: position saved" : "ERR: save failed";
        }
        if (cmd.startsWith("RECALL ")) {
            String label = cmd.substring(7);
            bool ok = tuner->recallByLabel(label.c_str());
            return ok ? "OK: recalled" : "ERR: label not found";
        }
        if (cmd.startsWith("SWEEP ")) {
            float a = 0, b = 0, s = 0.5f;
            sscanf(cmd.c_str() + 6, "%f %f %f", &a, &b, &s);
            tuner->startSweep(a, b, s);
            return "OK: sweep started";
        }
        if (cmd == "TABLE") {
            tuner->printLookupTable();
            return "OK: table printed";
        }
        if (cmd == "PRESETS") {
            tuner->printBandPresets();
            return "OK";
        }
        if (cmd == "LIST") {
            tuner->printSaved();
            return "OK";
        }
        if (cmd == "STOP") {
            pos->stepper->stop();
            pos->state = MOTOR_IDLE;
            return "OK: stopped";
        }
        if (cmd == "INFO") {
            return buildInfo();
        }
        if (cmd == "RESET") {
            ESP.restart();
            return ""; // Never reached
        }
        return "ERR: unknown command. Commands: HOME STATUS SETL GOTO GOSTEP STEP BAND FREQ SAVE RECALL SWEEP TABLE PRESETS LIST STOP INFO RESET";
    }

private:
    String buildStatus() {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "{\"l_uh\":%.3f,\"turns\":%.2f,\"pos_mm\":%.3f,\"steps\":%ld,"
            "\"state\":%d,\"homed\":%s,\"fault\":%s,\"fw\":\"%s\"}",
            pos->currentL(),
            pos->currentTurns(),
            pos->currentMM(),
            pos->position_steps,
            (int)pos->state,
            pos->isHomed() ? "true" : "false",
            pos->fault ? "true" : "false",
            FW_VERSION
        );
        return String(buf);
    }

    String buildInfo() {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "RF-RollerTuner fw=%s coil_d=%.2f\" turns=%d pitch=%.2fmm "
            "L_range=%.1f-%.1f uH steps_per_mm=%.0f",
            FW_VERSION,
            COIL_DIAMETER_IN,
            COIL_TURNS_TOTAL,
            COIL_PITCH_MM,
            L_MIN_UH, L_MAX_UH,
            STEPS_PER_MM
        );
        return String(buf);
    }
};
