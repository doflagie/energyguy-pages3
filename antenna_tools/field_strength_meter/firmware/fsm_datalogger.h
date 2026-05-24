/*
 * fsm_datalogger.h
 * SD card data logger and GPS interface for Field Strength Meter
 * TM-FSM-001 Rev A
 *
 * SD card: VSPI on GPIO 5(CS), 18(SCK), 19(MISO), 23(MOSI)
 * GPS:     UART2 on GPIO 16(RX), 17(TX), 9600 baud
 * Log format: CSV, one file per day, FAT32
 */

#pragma once
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

// ── SD Card ──────────────────────────────────────────────────────────────────
#define SD_CS   5
#define SD_SCK  18
#define SD_MISO 19
#define SD_MOSI 23

// ── GPS UART ─────────────────────────────────────────────────────────────────
#define GPS_RX  16
#define GPS_TX  17
#define GPS_BAUD 9600
#define GPS_UART 2   // Serial2

// ── LED ──────────────────────────────────────────────────────────────────────
#define LOG_LED_PIN 27   // blinks on SD write; 2× blink = GPS fix

// ── GPS State ─────────────────────────────────────────────────────────────────
struct GPSFix {
    bool   valid;
    float  lat;         // decimal degrees, positive = North
    float  lon;         // decimal degrees, positive = East
    float  alt_m;       // meters MSL
    int    satellites;
    char   utc_time[12]; // "HH:MM:SS"
    char   utc_date[12]; // "YYYY-MM-DD"
};

static GPSFix gps_fix = {false, 0, 0, 0, 0, "00:00:00", "1970-01-01"};

// ── Log State ─────────────────────────────────────────────────────────────────
static bool   sd_ready     = false;
static bool   gps_ready    = false;
static char   log_filename[32];
static int    log_entry_count = 0;
static SPIClass sdSPI(VSPI);   // dedicated VSPI for SD

// ─────────────────────────────────────────────────────────────────────────────
// NMEA parsing helpers
// ─────────────────────────────────────────────────────────────────────────────

static float nmea_to_decimal(const char* nmea_pos, char hemi) {
    // NMEA: DDMM.MMMMM (lat) or DDDMM.MMMMM (lon)
    float raw = atof(nmea_pos);
    int degrees = (int)(raw / 100);
    float minutes = raw - degrees * 100.0f;
    float decimal = degrees + minutes / 60.0f;
    if (hemi == 'S' || hemi == 'W') decimal = -decimal;
    return decimal;
}

static void parse_nmea_rmc(const char* sentence) {
    // $GPRMC,HHMMSS.ss,A,LLLL.LL,a,YYYYY.YY,a,x.x,x.x,DDMMYY,...
    char buf[128];
    strncpy(buf, sentence, sizeof(buf) - 1);
    buf[sizeof(buf)-1] = '\0';

    char* tok = strtok(buf, ",");
    int field = 0;
    char time_raw[12] = "";
    char status = 'V';
    char lat_raw[16] = "", lat_hemi = 'N';
    char lon_raw[16] = "", lon_hemi = 'E';
    char date_raw[8] = "";

    while (tok) {
        switch (field) {
            case 1:  strncpy(time_raw, tok, 11); break;
            case 2:  status = tok[0]; break;
            case 3:  strncpy(lat_raw, tok, 15); break;
            case 4:  lat_hemi = tok[0]; break;
            case 5:  strncpy(lon_raw, tok, 15); break;
            case 6:  lon_hemi = tok[0]; break;
            case 9:  strncpy(date_raw, tok, 7); break;
        }
        tok = strtok(NULL, ",");
        field++;
    }

    if (status == 'A' && strlen(lat_raw) > 0 && strlen(date_raw) == 6) {
        gps_fix.valid = true;
        gps_fix.lat = nmea_to_decimal(lat_raw, lat_hemi);
        gps_fix.lon = nmea_to_decimal(lon_raw, lon_hemi);

        // Format UTC time: HHMMSS.ss → HH:MM:SS
        snprintf(gps_fix.utc_time, sizeof(gps_fix.utc_time), "%c%c:%c%c:%c%c",
                 time_raw[0], time_raw[1], time_raw[2],
                 time_raw[3], time_raw[4], time_raw[5]);

        // Format date: DDMMYY → YYYY-MM-DD (assumes 2000s)
        snprintf(gps_fix.utc_date, sizeof(gps_fix.utc_date), "20%c%c-%c%c-%c%c",
                 date_raw[4], date_raw[5],
                 date_raw[2], date_raw[3],
                 date_raw[0], date_raw[1]);

        // Build log filename from date
        snprintf(log_filename, sizeof(log_filename),
                 "/FSM_%c%c%c%c%c%c.CSV",
                 date_raw[4], date_raw[5],   // YY
                 date_raw[2], date_raw[3],   // MM
                 date_raw[0], date_raw[1]);  // DD
    }
}

static void parse_nmea_gga(const char* sentence) {
    // $GPGGA,HHMMSS.ss,lat,N,lon,E,quality,sats,hdop,alt,M,...
    char buf[128];
    strncpy(buf, sentence, sizeof(buf) - 1);

    char* tok = strtok(buf, ",");
    int field = 0;
    while (tok) {
        switch (field) {
            case 7:  gps_fix.satellites = atoi(tok); break;
            case 9:  gps_fix.alt_m = atof(tok); break;
        }
        tok = strtok(NULL, ",");
        field++;
    }
}

// Validate NMEA checksum ($...*XX)
static bool nmea_checksum_ok(const char* sentence) {
    if (sentence[0] != '$') return false;
    const char* p = sentence + 1;
    uint8_t calc = 0;
    while (*p && *p != '*') calc ^= (uint8_t)(*p++);
    if (*p != '*') return false;
    uint8_t given = (uint8_t)strtol(p + 1, NULL, 16);
    return calc == given;
}

// ─────────────────────────────────────────────────────────────────────────────
// GPS poll (call in loop, non-blocking)
// ─────────────────────────────────────────────────────────────────────────────

static char gps_buf[128];
static int  gps_buf_pos = 0;

void gps_poll() {
    while (Serial2.available()) {
        char c = (char)Serial2.read();
        if (c == '$') {
            gps_buf_pos = 0;
        }
        if (gps_buf_pos < (int)sizeof(gps_buf) - 1) {
            gps_buf[gps_buf_pos++] = c;
            gps_buf[gps_buf_pos]   = '\0';
        }
        if (c == '\n' && gps_buf_pos > 6) {
            if (nmea_checksum_ok(gps_buf)) {
                if (strncmp(gps_buf, "$GPRMC", 6) == 0) parse_nmea_rmc(gps_buf);
                else if (strncmp(gps_buf, "$GPGGA", 6) == 0) parse_nmea_gga(gps_buf);
            }
            gps_buf_pos = 0;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// SD card initialization
// ─────────────────────────────────────────────────────────────────────────────

bool logger_sd_init() {
    sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, sdSPI, 4000000)) {
        sd_ready = false;
        return false;
    }
    sd_ready = true;

    // Default filename until GPS gives us the date
    strlcpy(log_filename, "/FSM_LOG.CSV", sizeof(log_filename));

    // Write CSV header if new file
    if (!SD.exists(log_filename)) {
        File f = SD.open(log_filename, FILE_WRITE);
        if (f) {
            f.println("UTC_Time,Date,Lat_deg,Lon_deg,Alt_m,Sats,"
                      "Band,Freq_MHz,Atten_dB,dBm,V_rms_uV,E_Vm,"
                      "S_units,Notes");
            f.close();
        }
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// GPS initialization
// ─────────────────────────────────────────────────────────────────────────────

void logger_gps_init() {
    Serial2.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    gps_ready = true;
    pinMode(LOG_LED_PIN, OUTPUT);
    digitalWrite(LOG_LED_PIN, LOW);
}

// ─────────────────────────────────────────────────────────────────────────────
// LED blink helper (non-blocking, uses millis)
// ─────────────────────────────────────────────────────────────────────────────

static unsigned long led_blink_time = 0;
static int led_blink_count = 0;
static bool led_blink_active = false;

void led_blink(int count) {
    led_blink_count  = count * 2;   // on/off pairs
    led_blink_time   = millis();
    led_blink_active = true;
}

void led_blink_update() {
    if (!led_blink_active) return;
    if (millis() - led_blink_time > 100) {
        led_blink_time = millis();
        led_blink_count--;
        digitalWrite(LOG_LED_PIN, (led_blink_count % 2 == 1) ? HIGH : LOW);
        if (led_blink_count <= 0) {
            led_blink_active = false;
            digitalWrite(LOG_LED_PIN, LOW);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Write one log entry
// ─────────────────────────────────────────────────────────────────────────────

struct FSMReading {
    float dBm;
    float V_rms_uV;  // probe voltage, microvolts
    float E_Vm;      // field strength, V/m
    float s_units;   // S-meter value (float, e.g. 6.5)
    const char* band_name;
    float freq_MHz;
    int   atten_dB;
    const char* notes;
};

bool logger_write(const FSMReading& r) {
    if (!sd_ready) return false;

    // Re-open file each write (safe for remove/insert scenarios)
    File f = SD.open(log_filename, FILE_APPEND);
    if (!f) {
        // Try to create header if file doesn't exist
        f = SD.open(log_filename, FILE_WRITE);
        if (!f) return false;
        f.println("UTC_Time,Date,Lat_deg,Lon_deg,Alt_m,Sats,"
                  "Band,Freq_MHz,Atten_dB,dBm,V_rms_uV,E_Vm,"
                  "S_units,Notes");
    }

    char line[256];
    snprintf(line, sizeof(line),
        "%s,%s,%.6f,%.6f,%.1f,%d,"
        "%s,%.4f,%d,%.2f,%.3f,%.4f,"
        "%.1f,%s",
        gps_fix.utc_time,
        gps_fix.utc_date,
        gps_fix.lat,
        gps_fix.lon,
        gps_fix.alt_m,
        gps_fix.satellites,
        r.band_name,
        r.freq_MHz,
        r.atten_dB,
        r.dBm,
        r.V_rms_uV,
        r.E_Vm,
        r.s_units,
        r.notes ? r.notes : "");

    f.println(line);
    f.close();

    log_entry_count++;
    led_blink(1);   // single blink = log written
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
// Read last N lines from log (for history screen display)
// Returns lines into buffer array; caller allocates char[N][maxLen]
// ─────────────────────────────────────────────────────────────────────────────

int logger_read_last(char lines[][96], int n, int maxLen) {
    if (!sd_ready || !SD.exists(log_filename)) return 0;

    // Count total lines
    File f = SD.open(log_filename, FILE_READ);
    if (!f) return 0;

    int total_lines = 0;
    while (f.available()) {
        if (f.read() == '\n') total_lines++;
    }
    f.close();

    // Skip to (total_lines - n) then read last n
    f = SD.open(log_filename, FILE_READ);
    if (!f) return 0;

    int skip_lines = max(0, total_lines - n - 1);  // −1 for header
    int skipped = 0;
    while (f.available() && skipped < skip_lines) {
        char c = f.read();
        if (c == '\n') skipped++;
    }

    int read_count = 0;
    while (f.available() && read_count < n) {
        String line = f.readStringUntil('\n');
        if (line.length() > 0) {
            strncpy(lines[read_count], line.c_str(), maxLen - 1);
            lines[read_count][maxLen - 1] = '\0';
            read_count++;
        }
    }
    f.close();
    return read_count;
}

// ─────────────────────────────────────────────────────────────────────────────
// Status helpers
// ─────────────────────────────────────────────────────────────────────────────

bool logger_sd_ok()    { return sd_ready; }
bool logger_gps_ok()   { return gps_fix.valid; }
int  logger_count()    { return log_entry_count; }
const GPSFix& logger_gps() { return gps_fix; }
const char* logger_filename() { return log_filename; }

// Update filename when GPS provides a new date
void logger_update_filename() {
    if (!gps_fix.valid) return;
    // log_filename already set in parse_nmea_rmc
    // Open and write header if the new-day file doesn't exist
    if (!SD.exists(log_filename)) {
        File f = SD.open(log_filename, FILE_WRITE);
        if (f) {
            f.println("UTC_Time,Date,Lat_deg,Lon_deg,Alt_m,Sats,"
                      "Band,Freq_MHz,Atten_dB,dBm,V_rms_uV,E_Vm,"
                      "S_units,Notes");
            f.close();
        }
    }
}
