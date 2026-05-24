#pragma once
// ============================================================
// bearing.h — TM-ADF-001 Rev A
// GPS + Compass bearing reference subsystem
// Header-only implementation
// ============================================================

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <math.h>
#include <nvs_flash.h>
#include <nvs.h>

struct GPSFix {
    bool    valid;
    float   lat;           // decimal degrees, + = North
    float   lon;           // decimal degrees, + = East
    float   altitude_m;
    float   speed_kmh;
    float   course_deg;    // GPS track
    uint8_t satellites;
    char    time_str[10];  // "HH:MM:SS"
    char    date_str[7];   // "DDMMYY"
    uint32_t timestamp_ms;
};

struct CompassReading {
    float   heading_mag;   // 0–360°, magnetic North
    float   heading_true;  // After declination
    float   pitch_deg;
    float   roll_deg;
    int16_t raw_x, raw_y, raw_z;
    bool    calibrated;
};

class BearingDisplay {
public:
    BearingDisplay()
        : _gps_serial(nullptr), _wire(nullptr), _calibrating(false),
          _hard_iron_x(0), _hard_iron_y(0),
          _cal_min_x(32767), _cal_max_x(-32768),
          _cal_min_y(32767), _cal_max_y(-32768), _cal_samples(0) {
        memset(&_fix, 0, sizeof(_fix));
        memset(&_compass, 0, sizeof(_compass));
        _nmea_pos = 0;
    }

    bool beginGPS(HardwareSerial& serial, uint32_t baud = 9600) {
        _gps_serial = &serial;
        _gps_serial->begin(baud);
        return true;
    }

    bool beginCompass(TwoWire& wire = Wire) {
        _wire = &wire;
        // Soft reset QMC5883L
        _writeReg(COMPASS_I2C_ADDR, 0x0A, 0x80);
        delay(10);
        // Set/reset period
        _writeReg(COMPASS_I2C_ADDR, 0x0B, 0x01);
        // Control 1: OSR=256, RNG=±2G, ODR=10Hz, MODE=continuous
        _writeReg(COMPASS_I2C_ADDR, 0x09, 0x1D);
        delay(10);
        return true;
    }

    void updateGPS() {
        if (!_gps_serial) return;
        while (_gps_serial->available()) {
            char c = (char)_gps_serial->read();
            if (c == '$') {
                _nmea_pos = 0;
                _nmea_buf[_nmea_pos++] = c;
            } else if (c == '\n' || c == '\r') {
                if (_nmea_pos > 5) {
                    _nmea_buf[_nmea_pos] = '\0';
                    _parseNMEA(_nmea_buf);
                }
                _nmea_pos = 0;
            } else if (_nmea_pos < 127) {
                _nmea_buf[_nmea_pos++] = c;
            }
        }
    }

    void updateCompass() {
        if (!_wire) return;
        int16_t x, y, z;
        if (!_readXYZ(x, y, z)) return;
        _compass.raw_x = x;
        _compass.raw_y = y;
        _compass.raw_z = z;

        // Hard-iron correction
        float xc = (float)(x - _hard_iron_x);
        float yc = (float)(y - _hard_iron_y);

        // Update calibration if in progress
        if (_calibrating) {
            if (x < _cal_min_x) _cal_min_x = x;
            if (x > _cal_max_x) _cal_max_x = x;
            if (y < _cal_min_y) _cal_min_y = y;
            if (y > _cal_max_y) _cal_max_y = y;
            _cal_samples++;
        }

        // Heading (flat, no tilt compensation)
        float heading = atan2f(-yc, xc) * 180.0f / (float)M_PI;
        if (heading < 0.0f) heading += 360.0f;
        _compass.heading_mag = heading;
        _compass.heading_true = heading + getMagneticDeclination(_fix.lat, _fix.lon);
        if (_compass.heading_true < 0.0f)   _compass.heading_true += 360.0f;
        if (_compass.heading_true >= 360.0f) _compass.heading_true -= 360.0f;
        _compass.calibrated = (_cal_samples > 50);
    }

    GPSFix        getGPSFix()  const { return _fix; }
    bool          hasGPSFix()  const { return _fix.valid; }
    CompassReading getCompass() const { return _compass; }

    // Returns approximate magnetic declination in degrees for given lat/lon
    // Positive = East (add to magnetic to get true)
    float getMagneticDeclination(float lat, float lon) {
        // Very simplified: western hemisphere = mostly east declination (positive),
        // eastern US coast = west declination (negative)
        // For accurate values, load WMM2025 coefficients; this is a rough estimate.
        if (lat > 20.0f && lat < 60.0f && lon < -60.0f && lon > -130.0f) {
            // Continental USA: interpolate based on longitude
            // West coast ~+14°, East coast ~-13°
            float t = (lon + 130.0f) / 70.0f;  // 0=west, 1=east
            return 14.0f - 27.0f * t;
        }
        if (lat > 48.0f && lat < 62.0f && lon > -12.0f && lon < 30.0f) {
            return 1.5f;  // UK/W.Europe approximate
        }
        return 0.0f;  // Default: no correction
    }

    // Haversine bearing from (lat1,lon1) to (lat2,lon2), degrees
    float bearingToWaypoint(float lat1, float lon1, float lat2, float lon2) {
        float lat1r = lat1 * (float)M_PI / 180.0f;
        float lat2r = lat2 * (float)M_PI / 180.0f;
        float dlon  = (lon2 - lon1) * (float)M_PI / 180.0f;
        float y = sinf(dlon) * cosf(lat2r);
        float x = cosf(lat1r) * sinf(lat2r) - sinf(lat1r) * cosf(lat2r) * cosf(dlon);
        float b = atan2f(y, x) * 180.0f / (float)M_PI;
        if (b < 0.0f) b += 360.0f;
        return b;
    }

    // Great-circle distance in meters
    float distanceToWaypoint_m(float lat1, float lon1, float lat2, float lon2) {
        const float R = 6371000.0f;
        float dlat = (lat2 - lat1) * (float)M_PI / 180.0f;
        float dlon = (lon2 - lon1) * (float)M_PI / 180.0f;
        float a = sinf(dlat/2)*sinf(dlat/2) +
                  cosf(lat1*(float)M_PI/180.0f)*cosf(lat2*(float)M_PI/180.0f)*
                  sinf(dlon/2)*sinf(dlon/2);
        float c = 2.0f * atan2f(sqrtf(a), sqrtf(1.0f-a));
        return R * c;
    }

    float magneticToTrue(float mag, float lat, float lon) {
        float t = mag + getMagneticDeclination(lat, lon);
        if (t <   0.0f) t += 360.0f;
        if (t >= 360.0f) t -= 360.0f;
        return t;
    }

    void formatBearing(float bearing, char* buf, size_t len) {
        const char* dirs[] = {"N","NNE","NE","ENE","E","ESE","SE","SSE",
                               "S","SSW","SW","WSW","W","WNW","NW","NNW"};
        int idx = (int)((bearing + 11.25f) / 22.5f) % 16;
        snprintf(buf, len, "%03.0f\xB0 %s", bearing, dirs[idx]);
    }

    void formatLatLon(float lat, float lon, char* buf, size_t len) {
        char ns = lat >= 0 ? 'N' : 'S';
        char ew = lon >= 0 ? 'E' : 'W';
        float alat = fabsf(lat), alon = fabsf(lon);
        int dlat = (int)alat, dlon = (int)alon;
        float mlat = (alat - dlat) * 60.0f, mlon = (alon - dlon) * 60.0f;
        snprintf(buf, len, "%c%d\xB0%.2f' %c%d\xB0%.2f'", ns, dlat, mlat, ew, dlon, mlon);
    }

    void startCalibration() {
        _calibrating = true;
        _cal_min_x = 32767; _cal_max_x = -32768;
        _cal_min_y = 32767; _cal_max_y = -32768;
        _cal_samples = 0;
    }

    bool isCalibrating() const { return _calibrating; }

    void finishCalibration() {
        if (_cal_samples < 50) return;
        _hard_iron_x = (_cal_min_x + _cal_max_x) / 2;
        _hard_iron_y = (_cal_min_y + _cal_max_y) / 2;
        _calibrating = false;
        _compass.calibrated = true;
    }

    float getCalibrationProgress() const {
        // Consider 200 samples a full rotation
        return fminf((float)_cal_samples / 200.0f, 1.0f);
    }

private:
    static const uint8_t COMPASS_I2C_ADDR = 0x0D;

    GPSFix         _fix;
    CompassReading _compass;
    HardwareSerial* _gps_serial;
    TwoWire*        _wire;
    bool            _calibrating;
    int16_t         _hard_iron_x, _hard_iron_y;
    int16_t         _cal_min_x, _cal_max_x, _cal_min_y, _cal_max_y;
    int             _cal_samples;
    char            _nmea_buf[128];
    uint8_t         _nmea_pos;

    void _writeReg(uint8_t addr, uint8_t reg, uint8_t val) {
        _wire->beginTransmission(addr);
        _wire->write(reg);
        _wire->write(val);
        _wire->endTransmission();
    }

    bool _readXYZ(int16_t& x, int16_t& y, int16_t& z) {
        _wire->beginTransmission(COMPASS_I2C_ADDR);
        _wire->write(0x00);
        _wire->endTransmission(false);
        if (_wire->requestFrom((uint8_t)COMPASS_I2C_ADDR, (uint8_t)6) < 6) return false;
        uint8_t buf[6];
        for (int i = 0; i < 6; i++) buf[i] = _wire->read();
        x = (int16_t)(buf[1] << 8 | buf[0]);
        y = (int16_t)(buf[3] << 8 | buf[2]);
        z = (int16_t)(buf[5] << 8 | buf[4]);
        return true;
    }

    bool _parseNMEA(const char* s) {
        if (strncmp(s, "$GNGGA", 6) == 0 || strncmp(s, "$GPGGA", 6) == 0) return _parseGGA(s);
        if (strncmp(s, "$GNRMC", 6) == 0 || strncmp(s, "$GPRMC", 6) == 0) return _parseRMC(s);
        return false;
    }

    // Extract field n from comma-separated NMEA sentence
    static void _field(const char* s, int n, char* out, size_t olen) {
        out[0] = '\0';
        int f = 0; size_t j = 0;
        for (size_t i = 0; s[i] && s[i] != '*'; i++) {
            if (s[i] == ',') { f++; j = 0; continue; }
            if (f == n && j < olen-1) out[j++] = s[i];
        }
        out[j] = '\0';
    }

    bool _parseGGA(const char* s) {
        char buf[20];
        _field(s, 6, buf, sizeof(buf));  // fix quality
        if (buf[0] == '0' || buf[0] == '\0') { _fix.valid = false; return false; }
        _field(s, 2, buf, sizeof(buf)); _fix.lat = _latlon(buf);
        _field(s, 3, buf, sizeof(buf)); if (buf[0]=='S') _fix.lat = -_fix.lat;
        _field(s, 4, buf, sizeof(buf)); _fix.lon = _latlon(buf);
        _field(s, 5, buf, sizeof(buf)); if (buf[0]=='W') _fix.lon = -_fix.lon;
        _field(s, 7, buf, sizeof(buf)); _fix.satellites = atoi(buf);
        _field(s, 9, buf, sizeof(buf)); _fix.altitude_m = atof(buf);
        _field(s, 1, buf, sizeof(buf));
        if (strlen(buf) >= 6) snprintf(_fix.time_str, sizeof(_fix.time_str),
            "%c%c:%c%c:%c%c", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
        _fix.valid = true;
        _fix.timestamp_ms = millis();
        return true;
    }

    bool _parseRMC(const char* s) {
        char buf[20];
        _field(s, 2, buf, sizeof(buf));
        if (buf[0] != 'A') return false;  // not active
        _field(s, 7, buf, sizeof(buf)); _fix.speed_kmh = atof(buf) * 1.852f;
        _field(s, 8, buf, sizeof(buf)); _fix.course_deg = atof(buf);
        _field(s, 9, buf, sizeof(buf)); strncpy(_fix.date_str, buf, sizeof(_fix.date_str));
        return true;
    }

    // Convert NMEA DDDMM.MMMM to decimal degrees
    static float _latlon(const char* s) {
        if (!s || s[0] == '\0') return 0.0f;
        float v = atof(s);
        int deg = (int)(v / 100);
        float min = v - deg * 100.0f;
        return deg + min / 60.0f;
    }
};
