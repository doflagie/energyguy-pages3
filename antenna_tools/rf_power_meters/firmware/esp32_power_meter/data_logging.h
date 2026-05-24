/**
 * data_logging.h — TM-PWR-ESP32-001 Rev A
 * SD card data logging, USB serial CSV, Bluetooth JSON streaming
 */

#pragma once
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include "config.h"
#include "calibration.h"

// ─── CSV Header ──────────────────────────────────────────────────────────────
static const char* LOG_CSV_HEADER =
    "timestamp_ms,freq_mhz,fwd_w,ref_w,peak_w,swr,rl_db,gamma,eff_pct,"
    "vfwd_mv,vref_mv,vpeak_mv,temp_c,range,band\r\n";

// ─── Logger Class ─────────────────────────────────────────────────────────────

class DataLogger {
public:
    bool    sd_available  = false;
    bool    sd_logging    = false;
    bool    bt_streaming  = false;
    uint32_t log_count    = 0;
    uint32_t last_log_ms  = 0;
    char     filename[32] = {0};

    bool initSD() {
        SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
        if (!SD.begin(SD_CS_PIN)) {
            Serial.println("SD: card not found");
            sd_available = false;
            return false;
        }
        sd_available = true;
        _makeFilename();
        // Write header
        File f = SD.open(filename, FILE_WRITE);
        if (f) {
            f.print(LOG_CSV_HEADER);
            f.close();
            Serial.printf("SD: logging to %s\n", filename);
        }
        return true;
    }

    // Toggle SD logging on/off
    void toggleLogging() {
        if (!sd_available) { Serial.println("SD not available"); return; }
        sd_logging = !sd_logging;
        Serial.printf("SD logging: %s\n", sd_logging ? "ON" : "OFF");
    }

    // Log one reading — call from main loop at SD_LOG_INTERVAL_MS
    void logReading(const PowerReading& r, float freq_mhz) {
        if (millis() - last_log_ms < SD_LOG_INTERVAL_MS) return;
        last_log_ms = millis();

        char line[256];
        snprintf(line, sizeof(line),
            "%lu,%.3f,%.4f,%.4f,%.4f,%.3f,%.2f,%.4f,%.1f,"
            "%.2f,%.2f,%.2f,%.1f,%d,%s\r\n",
            (unsigned long)r.ts_ms,
            freq_mhz,
            r.fwd_w, r.ref_w, r.peak_w,
            r.swr, r.rl_db, r.gamma,
            r.valid ? (1.0f - r.ref_w / max(r.fwd_w, 0.001f)) * 100.0f : 0.0f,
            r.vfwd_mv, r.vref_mv, r.vpeak_mv,
            r.temp_c, r.range,
            BAND_STR
        );

        // USB Serial (always)
        Serial.print(line);

        // SD card
        if (sd_logging && sd_available) {
            File f = SD.open(filename, FILE_APPEND);
            if (f) {
                f.print(line);
                f.close();
                log_count++;
                // Roll to new file if size exceeded
                if (log_count % 1000 == 0) _checkFileSize();
            }
        }

        // Bluetooth JSON (if streaming enabled)
        if (bt_streaming) {
            _sendBTJson(r, freq_mhz);
        }
    }

    // Send JSON status via Bluetooth
    void sendStatus(BluetoothSerial& btSerial, const PowerReading& r, float freq_mhz) {
        StaticJsonDocument<384> doc;
        doc["fwd_w"]    = serialized(String(r.fwd_w, 3));
        doc["ref_w"]    = serialized(String(r.ref_w, 3));
        doc["peak_w"]   = serialized(String(r.peak_w, 3));
        doc["swr"]      = serialized(String(r.swr, 3));
        doc["rl_db"]    = serialized(String(r.rl_db, 2));
        doc["freq_mhz"] = freq_mhz;
        doc["temp_c"]   = serialized(String(r.temp_c, 1));
        doc["range"]    = RANGE_LABEL[r.range];
        doc["band"]     = BAND_STR;
        doc["sd_log"]   = sd_logging;
        doc["log_n"]    = log_count;
        doc["fw"]       = FW_VERSION;
        String out;
        serializeJson(doc, out);
        btSerial.println(out);
        Serial.println(out);
    }

    bool isLogging() const { return sd_logging; }
    uint32_t getCount() const { return log_count; }
    const char* getFilename() const { return filename; }

private:
    void _makeFilename() {
        // Filename: /pwrmtr_NNNNN.csv where N increments to avoid overwrite
        int n = 0;
        do {
            snprintf(filename, sizeof(filename), "%s%05d.csv", SD_FILENAME_PREFIX, n++);
        } while (SD.exists(filename) && n < 99999);
    }

    void _checkFileSize() {
        File f = SD.open(filename, FILE_READ);
        if (f) {
            uint32_t sz = f.size();
            f.close();
            if (sz > SD_MAX_FILE_SIZE) {
                _makeFilename();
                File nf = SD.open(filename, FILE_WRITE);
                if (nf) { nf.print(LOG_CSV_HEADER); nf.close(); }
                log_count = 0;
                Serial.printf("SD: rolled to %s\n", filename);
            }
        }
    }

    void _sendBTJson(const PowerReading& r, float freq_mhz) {
        // Compact JSON for BT bandwidth
        char buf[128];
        snprintf(buf, sizeof(buf),
            "{\"f\":%.3f,\"fw\":%.3f,\"re\":%.3f,\"pk\":%.3f,\"sw\":%.2f}\n",
            freq_mhz, r.fwd_w, r.ref_w, r.peak_w, r.swr);
        // BT serial must be passed in by caller — use global or pass reference
        Serial.print(buf);
    }
};


// ─── Sweep Logger (frequency sweep mode) ──────────────────────────────────────

struct SweepPoint {
    float freq_mhz;
    float fwd_w;
    float ref_w;
    float swr;
};

class SweepLogger {
public:
    static const int MAX_POINTS = 200;
    SweepPoint points[MAX_POINTS];
    int  n_points   = 0;
    bool active     = false;
    float start_mhz = 1.8f;
    float stop_mhz  = 30.0f;
    float step_mhz  = 0.1f;
    float current_mhz = 0.0f;

    void start(float f_start, float f_stop, float f_step) {
        start_mhz   = f_start;
        stop_mhz    = f_stop;
        step_mhz    = f_step;
        current_mhz = f_start;
        n_points    = 0;
        active      = true;
        Serial.printf("SWEEP: %.3f–%.3f MHz step %.3f MHz\n",
                      f_start, f_stop, f_step);
    }

    void addPoint(float freq, float fwd_w, float ref_w, float swr) {
        if (!active || n_points >= MAX_POINTS) return;
        points[n_points++] = {freq, fwd_w, ref_w, swr};
        current_mhz += step_mhz;
        if (current_mhz > stop_mhz) {
            active = false;
            _printCSV();
        }
    }

    void saveToSD(DataLogger& dl) {
        if (!dl.sd_available) return;
        char fname[32];
        snprintf(fname, sizeof(fname), "/sweep_%05lu.csv", millis()/1000);
        File f = SD.open(fname, FILE_WRITE);
        if (!f) return;
        f.println("freq_mhz,fwd_w,ref_w,swr");
        for (int i = 0; i < n_points; i++) {
            f.printf("%.3f,%.4f,%.4f,%.3f\n",
                points[i].freq_mhz, points[i].fwd_w,
                points[i].ref_w,   points[i].swr);
        }
        f.close();
        Serial.printf("SWEEP saved: %s (%d points)\n", fname, n_points);
    }

private:
    void _printCSV() {
        Serial.println("freq_mhz,fwd_w,ref_w,swr");
        for (int i = 0; i < n_points; i++)
            Serial.printf("%.3f,%.4f,%.4f,%.3f\n",
                points[i].freq_mhz, points[i].fwd_w,
                points[i].ref_w, points[i].swr);
        Serial.println("SWEEP_END");
    }
};
