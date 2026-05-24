/**
 * tuner_protocol.h — TM-TUNER-001 Rev A
 * JSON Protocol between ESP32 Antenna Tuner and NEC Web API
 *
 * All messages are newline-terminated JSON over HTTP (API) or BT Serial.
 *
 * HTTP API requests use POST /model/run for prediction.
 * Status reports broadcast via BT Serial at POLL_INTERVAL_MS.
 */

#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

// ─── API Request: Get Predicted Tune Settings ─────────────────────────────────
// POST /tune/predict
// {
//   "action":       "get_tune",
//   "freq_mhz":     14.225,
//   "antenna_type": "dipole",
//   "z_estimate":   {"r": 50.0, "x": 0.0}   // Optional impedance hint
// }

// ─── API Response: Predicted Settings ────────────────────────────────────────
// {
//   "l_tap":       4,           // 0-7, inductance tap
//   "c_steps":     120,         // 0-200, capacitor position
//   "predicted_swr": 1.20,
//   "confidence":  0.85,        // 0-1 model confidence
//   "notes":       "20m dipole typical feed 45+j12 at 14.2 MHz"
// }

// ─── BT Status Report ─────────────────────────────────────────────────────────
// Broadcast every POLL_INTERVAL_MS:
// {
//   "swr_fwd_v":   0.234,       // Forward detector voltage (raw)
//   "swr_ref_v":   0.021,
//   "swr":         1.18,
//   "l_tap":       4,
//   "c_steps":     118,
//   "freq_mhz":    14.225,
//   "bypassed":    false,
//   "status":      "TUNED",     // HOMING, TUNING, TUNED, BYPASS, FAULT
//   "timestamp_ms": 123456
// }

// ─── BT Commands (newline-terminated strings) ─────────────────────────────────
// "TUNE 14.225"        → tune to 14.225 MHz (NEC predict + fine-tune)
// "SWEEP 7.0 7.3 0.1"  → sweep band, store tune per freq
// "STOREBAND"          → save current tune to NVS at current freq
// "STATUS"             → immediate JSON status reply
// "HOME"               → home both stepper axes
// "BYPASS ON"          → engage bypass relay
// "BYPASS OFF"         → disengage bypass relay
// "GETMAP"             → dump NVS tune map as CSV: freq_khz,l_tap,c_steps
// "SETANT dipole"      → set antenna type for API prediction
// "SETANT vertical"
// "SETANT yagi"
// "CALZERO"            → zero SWR detector calibration (no RF)
// "CALREF 1.0"         → calibrate with dummy load (set reference SWR)
// "RESET"              → factory reset NVS

struct TuneRequest {
    float   freq_mhz;
    char    antenna_type[32];
    float   z_r;
    float   z_x;

    String toJSON() const {
        StaticJsonDocument<256> doc;
        doc["action"]       = "get_tune";
        doc["freq_mhz"]     = freq_mhz;
        doc["antenna_type"] = antenna_type;
        JsonObject z        = doc.createNestedObject("z_estimate");
        z["r"]              = z_r;
        z["x"]              = z_x;
        String out;
        serializeJson(doc, out);
        return out;
    }
};

struct TuneResponse {
    int   l_tap;
    int   c_steps;
    float predicted_swr;
    float confidence;
    char  notes[128];
    bool  valid;

    bool fromJSON(const String& json) {
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, json);
        if (err) { valid = false; return false; }
        l_tap          = doc["l_tap"]         | 0;
        c_steps        = doc["c_steps"]       | 100;
        predicted_swr  = doc["predicted_swr"] | 1.0f;
        confidence     = doc["confidence"]    | 0.5f;
        strlcpy(notes, doc["notes"] | "", sizeof(notes));
        valid = true;
        return true;
    }
};

struct TunerStatus {
    float  swr;
    float  fwd_v;
    float  ref_v;
    int    l_tap;
    int    c_steps;
    float  freq_mhz;
    bool   bypassed;
    char   status_str[16];
    uint32_t timestamp_ms;

    String toJSON() const {
        StaticJsonDocument<384> doc;
        doc["swr_fwd_v"]    = fwd_v;
        doc["swr_ref_v"]    = ref_v;
        doc["swr"]          = swr;
        doc["l_tap"]        = l_tap;
        doc["c_steps"]      = c_steps;
        doc["freq_mhz"]     = freq_mhz;
        doc["bypassed"]     = bypassed;
        doc["status"]       = status_str;
        doc["timestamp_ms"] = timestamp_ms;
        String out;
        serializeJson(doc, out);
        return out;
    }
};

// NVS tune map entry (8 bytes per entry)
struct TuneMapEntry {
    uint16_t freq_khz;
    uint8_t  l_tap;
    uint8_t  c_steps_hi8;  // high 8 bits (c_steps can be 0-200)
    uint8_t  c_steps_lo8;  // low 8 bits
    uint8_t  reserved[3];

    void setCapSteps(int steps) {
        c_steps_hi8 = (steps >> 8) & 0xFF;
        c_steps_lo8 = steps & 0xFF;
    }
    int getCapSteps() const {
        return (c_steps_hi8 << 8) | c_steps_lo8;
    }
};
