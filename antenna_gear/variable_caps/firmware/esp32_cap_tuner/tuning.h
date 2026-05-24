/**
 * tuning.h — TM-VCAP-ESP32-001 Rev A
 * Capacitor Auto-Tuning, Band Presets, Sweep, Command Parser
 */

#pragma once
#include <Arduino.h>
#include "config.h"
#include "position.h"

// ─── Band Presets ─────────────────────────────────────────────────────────────
// Nominal capacitance values for common antenna tuner applications.
// Actual values depend on antenna impedance — use as starting points.

struct CapBandPreset {
    const char* name;
    float       freq_mhz;
    float       c_series_pf;   // Series arm of L-network (lower C needed at HF)
    float       c_shunt_pf;    // Shunt arm (input side)
    float       c_low_pf;      // Range low end
    float       c_high_pf;     // Range high end
};

static const CapBandPreset CAP_BAND_PRESETS[] = {
    // HF bands — nominal series capacitance (pF) for L-network/T-network tuners
    // Use VC-HF100 (10–225 pF) for 160m–10m at ≤100W; VC-QRP for QRP; VC-TX for ≥200W
    { "160m",  1.90f,  220.0f, 190.0f, 100.0f, 225.0f },
    { "80m",   3.70f,  160.0f, 135.0f,  80.0f, 220.0f },
    { "60m",   5.30f,  100.0f,  85.0f,  50.0f, 180.0f },
    { "40m",   7.10f,   70.0f,  60.0f,  30.0f, 130.0f },
    { "30m",  10.10f,   50.0f,  42.0f,  20.0f,  90.0f },
    { "20m",  14.20f,   32.0f,  27.0f,  12.0f,  60.0f },
    { "17m",  18.10f,   24.0f,  20.0f,   8.0f,  45.0f },
    { "15m",  21.20f,   18.0f,  15.0f,   6.0f,  35.0f },
    { "12m",  24.90f,   14.0f,  12.0f,   5.0f,  28.0f },
    { "10m",  28.50f,   11.0f,   9.5f,   4.0f,  22.0f },
    // VHF bands — use VC-VHF (2–47 pF); values are stub-matching starting points
    // Actual value depends on stub length and load impedance; autotune will refine
    { "6m",   51.00f,   18.0f,  15.0f,   5.0f,  40.0f },
    { "2m",  146.00f,    8.5f,   7.0f,   2.0f,  20.0f },
    { "1.25m",222.00f,   5.5f,   4.5f,   2.0f,  12.0f },
    // UHF bands — use VC-UHF (0.5–12 pF); PCB-mount; minimize lead length <3mm
    { "70cm", 446.00f,   4.0f,   3.5f,   1.0f,  10.0f },
    { "33cm", 906.00f,   2.5f,   2.0f,   0.5f,   6.0f },
    { "20cm",1296.00f,   1.5f,   1.2f,   0.5f,   4.0f },
};
static const int N_CAP_PRESETS = sizeof(CAP_BAND_PRESETS) / sizeof(CapBandPreset);


// ─── Saved Positions ──────────────────────────────────────────────────────────

struct CapSavedPos {
    float   c_pf;
    long    steps;
    char    label[16];
    bool    valid;
};
static const int CAP_MAX_SAVED = 10;


// ─── Auto-Tuner ───────────────────────────────────────────────────────────────

class CapAutoTuner {
public:
    CapPositionManager* pos;
    CapSavedPos saved[CAP_MAX_SAVED];
    int n_saved = 0;

    // Sweep state
    bool  sweep_active    = false;
    float sweep_c_start   = CAP_C_MIN_PF;
    float sweep_c_stop    = CAP_C_MAX_PF;
    float sweep_c_step    = 10.0f;
    float sweep_c_current = 0.0f;

    void begin(CapPositionManager* p) { pos = p; }

    // Go to band preset by name
    bool goToNamedBand(const char* name) {
        for (int i = 0; i < N_CAP_PRESETS; i++) {
            if (strcasecmp(CAP_BAND_PRESETS[i].name, name) == 0) {
                pos->moveToC(CAP_BAND_PRESETS[i].c_series_pf);
                Serial.printf("PRESET: %s → %.0f pF\n",
                    name, CAP_BAND_PRESETS[i].c_series_pf);
                return true;
            }
        }
        return false;
    }

    // Nearest band preset by frequency
    void goToBandByFreq(float freq_mhz) {
        const CapBandPreset* best = nullptr;
        float min_dist = 9999.0f;
        for (int i = 0; i < N_CAP_PRESETS; i++) {
            float d = fabsf(CAP_BAND_PRESETS[i].freq_mhz - freq_mhz);
            if (d < min_dist) { min_dist = d; best = &CAP_BAND_PRESETS[i]; }
        }
        if (best) {
            pos->moveToC(best->c_series_pf);
            Serial.printf("PRESET by freq %.1fMHz: %s → %.0f pF\n",
                freq_mhz, best->name, best->c_series_pf);
        }
    }

    bool savePosition(const char* label) {
        if (n_saved >= CAP_MAX_SAVED || !pos->homed) return false;
        saved[n_saved] = { pos->currentC(), pos->pos_steps, {}, true };
        strncpy(saved[n_saved].label, label, 15);
        n_saved++;
        Serial.printf("SAVED[%d]: %s = %.1f pF\n", n_saved-1, label, pos->currentC());
        return true;
    }

    bool recallByLabel(const char* label) {
        for (int i = 0; i < n_saved; i++) {
            if (saved[i].valid && strcasecmp(saved[i].label, label) == 0) {
                pos->moveTo(saved[i].steps);
                Serial.printf("RECALL: %s → %.1f pF\n", label, saved[i].c_pf);
                return true;
            }
        }
        return false;
    }

    void startSweep(float c_start, float c_stop, float c_step) {
        if (!pos->homed) { Serial.println("ERR: not calibrated"); return; }
        sweep_c_start   = c_start;
        sweep_c_stop    = c_stop;
        sweep_c_step    = c_step;
        sweep_c_current = c_start;
        sweep_active    = true;
        pos->moveToC(c_start);
        Serial.printf("SWEEP: %.0f–%.0f pF step %.0f pF\n", c_start, c_stop, c_step);
    }

    void updateSweep() {
        if (!sweep_active || pos->isMoving()) return;
        Serial.printf("SWEEP_PT,%.1f,%.1f,%.1f\n",
            sweep_c_current, pos->currentC(), pos->potC());
        sweep_c_current += sweep_c_step;
        if (sweep_c_current > sweep_c_stop) {
            sweep_active = false;
            Serial.println("SWEEP_END");
            return;
        }
        pos->moveToC(sweep_c_current);
    }

    void printBandPresets() {
        Serial.println("BAND_PRESETS (series capacitance):");
        for (int i = 0; i < N_CAP_PRESETS; i++)
            Serial.printf("  %-5s %5.1fMHz  C=%.0fpF  [%.0f–%.0fpF]\n",
                CAP_BAND_PRESETS[i].name,
                CAP_BAND_PRESETS[i].freq_mhz,
                CAP_BAND_PRESETS[i].c_series_pf,
                CAP_BAND_PRESETS[i].c_low_pf,
                CAP_BAND_PRESETS[i].c_high_pf);
    }

    void printCapTable() {
        Serial.println("angle_deg,C_pF,steps,X_C_7MHz_ohm,X_C_14MHz_ohm");
        for (int ang = 0; ang <= 180; ang += 10) {
            float c    = pos->calc.angleToC((float)ang);
            long  s    = pos->calc.angleToSteps((float)ang);
            float xc7  = pos->calc.reactanceOhm(c, 7.0f);
            float xc14 = pos->calc.reactanceOhm(c, 14.0f);
            Serial.printf("%d,%.1f,%ld,%.1f,%.1f\n", ang, c, s, xc7, xc14);
        }
        Serial.println("TABLE_END");
    }

    void printVoltageTable() {
        Serial.println("Power ratings for this capacitor at key frequencies:");
        Serial.println("freq_mhz,C_min_pF,C_max_pF,Vpeak@C_min,Vpeak@C_max");
        float freqs[] = {1.8f, 3.5f, 7.0f, 14.0f, 21.0f, 28.0f};
        float power_w = 1000.0f;  // 1kW reference
        for (float f : freqs) {
            float xc_min = pos->calc.reactanceOhm(CAP_C_MIN_PF, f);
            float xc_max = pos->calc.reactanceOhm(CAP_C_MAX_PF, f);
            float vp_min = CapCalc::peakVoltsRF(power_w, xc_min);
            float vp_max = CapCalc::peakVoltsRF(power_w, xc_max);
            Serial.printf("%.1f,%.0f,%.0f,%.0f,%.0f\n",
                f, CAP_C_MIN_PF, CAP_C_MAX_PF, vp_min, vp_max);
        }
    }
};


// ─── Command Parser ───────────────────────────────────────────────────────────

class CapCommandParser {
public:
    CapPositionManager* pos;
    CapAutoTuner*       tuner;

    void begin(CapPositionManager* p, CapAutoTuner* t) { pos = p; tuner = t; }

    String process(const String& raw) {
        String cmd = raw;
        cmd.trim();
        cmd.toUpperCase();

        if (cmd == "HOME")          { pos->startHoming(); return "OK: homing"; }
        if (cmd == "CALMIN")        { pos->pot.calMin(); return "OK: cal_min stored"; }
        if (cmd == "CALMAX")        { pos->pot.calMax(); return "OK: cal_max stored"; }
        if (cmd == "CALPOT")        { pos->calibrateFromPot(); return "OK: sync from pot"; }
        if (cmd == "STATUS")        return buildStatus();
        if (cmd == "INFO")          return buildInfo();
        if (cmd == "PRESETS")       { tuner->printBandPresets(); return "OK"; }
        if (cmd == "TABLE")         { tuner->printCapTable(); return "OK"; }
        if (cmd == "VTABLE")        { tuner->printVoltageTable(); return "OK"; }
        if (cmd == "LIST")          { printSaved(); return "OK"; }
        if (cmd == "STOP")          { pos->stepper->stop(); pos->state = CAP_IDLE; return "OK: stopped"; }
        if (cmd == "RESET")         { ESP.restart(); return ""; }

        if (cmd.startsWith("SETC ") || cmd.startsWith("GOTO ")) {
            float c = cmd.substring(5).toFloat();
            pos->moveToC(c);
            return String("OK: moving to ") + c + " pF";
        }
        if (cmd.startsWith("STEP ")) {
            long d = cmd.substring(5).toInt();
            pos->moveRelative(d);
            return String("OK: step ") + d;
        }
        if (cmd.startsWith("BAND ")) {
            String b = cmd.substring(5); b.toLowerCase();
            bool ok = tuner->goToNamedBand(b.c_str());
            return ok ? "OK" : "ERR: unknown band";
        }
        if (cmd.startsWith("FREQ ")) {
            tuner->goToBandByFreq(cmd.substring(5).toFloat());
            return "OK";
        }
        if (cmd.startsWith("SAVE ")) {
            bool ok = tuner->savePosition(cmd.substring(5).c_str());
            return ok ? "OK: saved" : "ERR";
        }
        if (cmd.startsWith("RECALL ")) {
            bool ok = tuner->recallByLabel(cmd.substring(7).c_str());
            return ok ? "OK" : "ERR: not found";
        }
        if (cmd.startsWith("SWEEP ")) {
            float a=0,b=0,s=10.0f;
            sscanf(cmd.c_str()+6, "%f %f %f", &a, &b, &s);
            tuner->startSweep(a, b, s);
            return "OK: sweep started";
        }
        return "ERR: unknown command. HOME CALMIN CALMAX CALPOT STATUS SETC GOTO STEP BAND FREQ SAVE RECALL SWEEP TABLE VTABLE PRESETS LIST STOP INFO RESET";
    }

private:
    String buildStatus() {
        char buf[320];
        snprintf(buf, sizeof(buf),
            "{\"c_pf\":%.1f,\"angle\":%.2f,\"steps\":%ld,"
            "\"pot_c\":%.1f,\"pot_angle\":%.1f,\"pos_err\":%.2f,"
            "\"state\":%d,\"homed\":%s,\"fault\":%s,\"fw\":\"%s\"}",
            pos->currentC(), pos->currentAngle(), pos->pos_steps,
            pos->potC(), pos->potAngle(), pos->positionError(),
            (int)pos->state,
            pos->homed ? "true" : "false",
            pos->fault ? "true" : "false",
            FW_VERSION);
        return String(buf);
    }

    String buildInfo() {
        char buf[256];
        snprintf(buf, sizeof(buf),
            "RF-CapTuner fw=%s C=%.0f-%.0fpF V=%.0fkV "
            "rot=%.0fdeg steps_full=%ld deg/step=%.6f",
            FW_VERSION,
            CAP_C_MIN_PF, CAP_C_MAX_PF, CAP_VOLTAGE_KV,
            CAP_ROTATION_DEG, STEPS_FULL_ROT, DEG_PER_STEP);
        return String(buf);
    }

    void printSaved() {
        Serial.printf("SAVED: %d positions\n", tuner->n_saved);
        for (int i = 0; i < tuner->n_saved; i++)
            if (tuner->saved[i].valid)
                Serial.printf("  [%d] %s = %.1f pF\n",
                    i, tuner->saved[i].label, tuner->saved[i].c_pf);
    }
};
