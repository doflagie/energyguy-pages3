/*==============================================================================
  VHF/UHF REMOTE ELEMENT SWITCHING SYSTEM
  MAIN BEAM CONTROLLER FIRMWARE - CYD TOUCHSCREEN + BLE MESH

  Target: ESP32 with CYD 480x320 touchscreen (optional, can run headless)
  Supports: 6M (50 MHz) through 20cm (1296 MHz) element switching
  Protocol: BLE Mesh (AFM vendor opcodes) + WiFi REST API

  Features:
    - 12 pre-programmed beam patterns per band
    - Real-time SWR monitoring (AD8307 power detector or NanoVNA)
    - CYD touchscreen UI with band selector and pattern matrix
    - WiFi WebServer + REST API (/api, POST /control)
    - BLE Mesh: Extends existing AFM protocol with element control opcodes
    - EEPROM persistence: Last pattern/band saved in Preferences
    - Dual control: WiFi OR BLE Mesh OR USB serial

  Compiler: Arduino IDE 2.x, ESP32 Board Manager 2.0.14+
  Dependencies:
    - Arduino core for ESP32
    - esp-idf-lib (for BLE Mesh)
    - ArduinoJson (for REST API)
    - Adafruit GFX (if CYD display enabled)
    - TinyGPSPlus (optional, for satellite tracking)
    - Preferences (built-in)

  Pin Assignments (CYD 2432S028):
    GPIO2 (relay/element 0)
    GPIO4 (relay/element 1)
    GPIO5 (relay/element 2)
    GPIO17 (relay/element 3)
    GPIO34 (RF sensor input, ADC channel 6)
    GPIO35 (forward power ADC)
    GPIO36 (reflected power ADC)
    SDA=GPIO21, SCL=GPIO22 (I2C for OLED/display)
    TX=GPIO1, RX=GPIO3 (GPS or debug UART)

  BLE Mesh AFM Vendor Opcodes (extended from existing protocol):
    0x05 = AFM_OP_ELEMENT_SET {element_idx(1), state(1)} -> element control
    0x06 = AFM_OP_PATTERN_SET {pattern_id(1)} -> load named beam pattern
    0x07 = AFM_OP_BAND_SELECT {band_enum(1)} -> 6M|2M|125M|70CM|33CM|20CM
    0x11 = AFM_OP_ELEMENT_STATUS {pwr_dbm(1), swr(1), temp(1)} -> element state reply
    0x12 = AFM_OP_BEAM_STATUS {azimuth(2), elevation(2), gain_dbi(1)} -> array state

  REST API Endpoints:
    GET /api -> {band, pattern, elements: [state0, state1, ...], swr, power_db}
    POST /control?band=6m&pattern=baseline -> sets pattern, returns status
    POST /element?idx=0&state=on -> directly set element state
    GET /patterns?band=2m -> list available patterns for band

  Serial Protocol (9600 8N1):
    Commands: p<n>, s, l, h (pattern, status, list, help)
    Format: ASCII newline-terminated

  Date: 2026-04
  System: ant_gear remote element switching
==============================================================================*/

// ============== INCLUDES ==============

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <esp_ble_mesh_common.h>
#include <esp_ble_mesh_networking.h>

#ifdef HAVE_CYD_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  // Fallback to OLED if CYD unavailable
#endif

// ============== BAND ENUMERATION ==============

enum Band {
  BAND_6M = 0,
  BAND_2M = 1,
  BAND_125M = 2,
  BAND_70CM = 3,
  BAND_33CM = 4,
  BAND_20CM = 5,
  NUM_BANDS = 6
};

const char* band_names[NUM_BANDS] = {
  "6M", "2M", "1.25M", "70CM", "33CM", "20CM"
};

// ============== BEAM PATTERNS ==============

enum BeamPattern {
  PATTERN_OMNI = 0,
  PATTERN_BASELINE = 1,
  PATTERN_COMPACT = 2,
  PATTERN_REVERSE = 3,
  PATTERN_CARDIOID_N = 4,
  PATTERN_CARDIOID_E = 5,
  PATTERN_CARDIOID_S = 6,
  PATTERN_CARDIOID_W = 7,
  PATTERN_ENDFIRE_FWD = 8,
  PATTERN_ENDFIRE_REV = 9,
  PATTERN_GAIN_MAX = 10,
  PATTERN_F_B_MAX = 11,
  NUM_PATTERNS = 12
};

const char* pattern_names[NUM_PATTERNS] = {
  "OMNI", "BASELINE", "COMPACT", "REVERSE",
  "CARDIOID-N", "CARDIOID-E", "CARDIOID-S", "CARDIOID-W",
  "ENDFIRE-FWD", "ENDFIRE-REV", "GAIN-MAX", "F/B-MAX"
};

// ============== ELEMENT STATE DEFINITIONS ==============

#define NUM_ELEMENTS 4
#define REFLECTOR 0
#define DIR1 1
#define DIR2 2
#define DIR3 3

enum ElementState {
  ELEMENT_OFF = 0,      // Isolated (director mode, high impedance)
  ELEMENT_REFLECTOR = 1, // Shorted to boom (reflector mode)
  ELEMENT_DIRECTOR = 2,  // Free (director mode)
  ELEMENT_BYPASS = 3     // Shorted via bypass stub
};

// ============== PATTERN MATRIX ==============
// Defines element states for each pattern
// Format: [reflector, dir1, dir2, dir3]

const uint8_t pattern_matrix[NUM_PATTERNS][NUM_ELEMENTS] = {
  // OMNI: all directors active
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // BASELINE: all active
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // COMPACT: 3-element (refl + DE + dir1 only)
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_REFLECTOR, ELEMENT_REFLECTOR},
  // REVERSE: reflector becomes director
  {ELEMENT_REFLECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // CARDIOID-N
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_REFLECTOR},
  // CARDIOID-E
  {ELEMENT_DIRECTOR, ELEMENT_REFLECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // CARDIOID-S
  {ELEMENT_REFLECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // CARDIOID-W
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_REFLECTOR, ELEMENT_DIRECTOR},
  // ENDFIRE-FWD
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // ENDFIRE-REV
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // GAIN-MAX: all directors, max impedance matching
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR},
  // F/B-MAX: reflector optimized
  {ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_DIRECTOR, ELEMENT_REFLECTOR}
};

// ============== GPIO PIN ASSIGNMENTS ==============

#define GPIO_REFL 2
#define GPIO_DIR1 4
#define GPIO_DIR2 5
#define GPIO_DIR3 17

const uint8_t gpio_pins[NUM_ELEMENTS] = {GPIO_REFL, GPIO_DIR1, GPIO_DIR2, GPIO_DIR3};

#define ADC_FWD_POWER 35  // Forward RF power (AD8307 output)
#define ADC_REF_POWER 36  // Reflected RF power (AD8307 output)
#define ADC_TEMP_SENSOR 34 // Temperature sensor input (optional)

// ============== GLOBAL OBJECTS ==============

WebServer server(80);
Preferences prefs;
WiFiMode_t wifi_mode = WIFI_STA;

// Current state
Band current_band = BAND_6M;
BeamPattern current_pattern = PATTERN_BASELINE;
uint8_t element_state[NUM_ELEMENTS];
float swr_value = 1.0;
float power_fwd_dbm = 0.0;
float power_ref_dbm = -30.0;

// ============== SETUP ==============

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n\n=== VHF/UHF REMOTE ELEMENT SWITCHING SYSTEM ===");
  Serial.println("Main Beam Controller Firmware");
  Serial.println("Build: 2026-04");

  // Initialize GPIO outputs
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    pinMode(gpio_pins[i], OUTPUT);
    digitalWrite(gpio_pins[i], LOW);  // All relays OFF initially
  }

  // Initialize ADC inputs
  analogSetAttenuation(ADC_11db);  // Full range 0-3.6V

  // Load saved state from NVS
  prefs.begin("beam_ctrl", false);
  current_band = (Band)prefs.getUChar("band", BAND_6M);
  current_pattern = (BeamPattern)prefs.getUChar("pattern", PATTERN_BASELINE);
  prefs.end();

  Serial.printf("Restored: Band=%s, Pattern=%s\n", band_names[current_band], pattern_names[current_pattern]);

  // Set initial pattern
  applyPattern(current_pattern);

  // WiFi setup
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));
  WiFi.softAP("BeamController", "hamradio");
  WiFi.begin("YourSSID", "YourPassword");  // Connect to home WiFi

  Serial.println("WiFi AP: BeamController (192.168.4.1)");

  // Web server routes
  server.on("/api", HTTP_GET, handleAPI);
  server.on("/control", HTTP_POST, handleControl);
  server.on("/element", HTTP_POST, handleElement);
  server.on("/patterns", HTTP_GET, handlePatternList);
  server.on("/", HTTP_GET, handleRoot);
  server.begin();

  Serial.println("WebServer started on port 80");
  Serial.println("\nSerial Commands: p<n>(pattern), s(status), l(list), h(help)");
}

// ============== MAIN LOOP ==============

void loop() {
  server.handleClient();

  // Read RF sensors periodically
  static unsigned long last_sensor_read = 0;
  if (millis() - last_sensor_read > 1000) {
    readRFSensors();
    last_sensor_read = millis();
  }

  // Serial command processing
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    processSerialCommand(cmd);
  }

  delay(10);
}

// ============== PATTERN APPLICATION ==============

void applyPattern(BeamPattern pattern) {
  if (pattern >= NUM_PATTERNS) {
    Serial.println("Error: Invalid pattern");
    return;
  }

  current_pattern = pattern;

  // Extract element states from pattern matrix
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    element_state[i] = pattern_matrix[pattern][i];
  }

  // Apply to GPIO
  updateGPIOOutputs();

  // Save to EEPROM
  prefs.begin("beam_ctrl", false);
  prefs.putUChar("pattern", (uint8_t)pattern);
  prefs.end();

  Serial.printf("Pattern applied: %s\n", pattern_names[pattern]);
}

void updateGPIOOutputs() {
  // For relays: HIGH = energized = director (NO contact), LOW = reflector (NC contact)
  // For PIN diodes: HIGH = forward bias (connected), LOW = reverse bias (isolated)

  for (int i = 0; i < NUM_ELEMENTS; i++) {
    uint8_t gpio_pin = gpio_pins[i];

    switch (element_state[i]) {
      case ELEMENT_DIRECTOR:
        digitalWrite(gpio_pin, HIGH);  // Relay energized or PIN forward
        break;
      case ELEMENT_REFLECTOR:
        digitalWrite(gpio_pin, LOW);   // Relay off (NC) or PIN reverse
        break;
      case ELEMENT_OFF:
        digitalWrite(gpio_pin, LOW);   // Same as reflector for now
        break;
      default:
        digitalWrite(gpio_pin, LOW);
    }
  }

  // Allow settling time for relay contacts
  delay(50);
}

void setElementState(uint8_t element_idx, uint8_t state) {
  if (element_idx >= NUM_ELEMENTS) return;

  element_state[element_idx] = state;
  updateGPIOOutputs();

  Serial.printf("Element %d: state=%d\n", element_idx, state);
}

// ============== RF SENSOR READING ==============

void readRFSensors() {
  // AD8307 logarithmic detector outputs:
  // 25mV/dB output level (standard for log detectors)
  // 0 dBm @ ~2.4V output

  int adc_fwd = analogRead(ADC_FWD_POWER);
  int adc_ref = analogRead(ADC_REF_POWER);

  // Convert ADC counts (0-4095) to voltage (0-3.6V)
  float vdc_fwd = adc_fwd * 3.6 / 4095.0;
  float vdc_ref = adc_ref * 3.6 / 4095.0;

  // Convert voltage to dBm (25 mV/dB, 2.4V = 0 dBm)
  power_fwd_dbm = (vdc_fwd - 2.4) / 0.025;
  power_ref_dbm = (vdc_ref - 2.4) / 0.025;

  // Calculate SWR: SWR = (1 + Γ) / (1 - Γ), where Γ = sqrt(Pref/Pfwd)
  if (power_fwd_dbm > -20) {  // Only valid if forward power > -20 dBm
    float gamma = sqrt(pow(10, (power_ref_dbm - power_fwd_dbm) / 10.0));
    swr_value = (1.0 + gamma) / (1.0 - gamma);
    if (swr_value < 1.0) swr_value = 1.0;
    if (swr_value > 50.0) swr_value = 50.0;
  } else {
    swr_value = 1.0;  // No signal
  }
}

// ============== WEB API HANDLERS ==============

void handleAPI() {
  DynamicJsonDocument doc(512);
  doc["band"] = band_names[current_band];
  doc["pattern"] = pattern_names[current_pattern];
  doc["swr"] = swr_value;
  doc["power_fwd_dbm"] = power_fwd_dbm;
  doc["power_ref_dbm"] = power_ref_dbm;

  JsonArray elements = doc.createNestedArray("elements");
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    elements.add(element_state[i]);
  }

  String response;
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

void handleControl() {
  if (server.hasArg("band") && server.hasArg("pattern")) {
    String band_str = server.arg("band");
    String pattern_str = server.arg("pattern");

    // Parse band
    Band new_band = BAND_6M;
    for (int i = 0; i < NUM_BANDS; i++) {
      if (band_str == band_names[i]) {
        new_band = (Band)i;
        break;
      }
    }

    // Parse pattern
    BeamPattern new_pattern = PATTERN_BASELINE;
    for (int i = 0; i < NUM_PATTERNS; i++) {
      if (pattern_str == pattern_names[i]) {
        new_pattern = (BeamPattern)i;
        break;
      }
    }

    current_band = new_band;
    applyPattern(new_pattern);

    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing band or pattern parameter");
  }
}

void handleElement() {
  if (server.hasArg("idx") && server.hasArg("state")) {
    int idx = server.arg("idx").toInt();
    int state = server.arg("state").toInt();

    setElementState(idx, state);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing idx or state parameter");
  }
}

void handlePatternList() {
  String band_filter = server.arg("band");

  DynamicJsonDocument doc(256);
  JsonArray patterns = doc.createNestedArray("patterns");

  for (int i = 0; i < NUM_PATTERNS; i++) {
    patterns.add(pattern_names[i]);
  }

  String response;
  serializeJson(doc, response);

  server.send(200, "application/json", response);
}

void handleRoot() {
  String html = R"(
<html>
<head><title>Beam Controller</title></head>
<body>
<h1>VHF/UHF Remote Element Switching</h1>
<p>API: /api (GET) - Current status</p>
<p>/control?band=6m&pattern=baseline (POST) - Set pattern</p>
<p>/element?idx=0&state=1 (POST) - Set element state</p>
</body>
</html>
  )";
  server.send(200, "text/html", html);
}

// ============== SERIAL COMMAND PROCESSING ==============

void processSerialCommand(String cmd) {
  if (cmd.length() == 0) return;

  char operation = cmd[0];

  switch (operation) {
    case 'p': {  // Pattern
      int pattern_num = cmd.substring(1).toInt();
      if (pattern_num >= 0 && pattern_num < NUM_PATTERNS) {
        applyPattern((BeamPattern)pattern_num);
      } else {
        Serial.println("Invalid pattern number");
      }
      break;
    }
    case 's': {  // Status
      printStatus();
      break;
    }
    case 'l': {  // List patterns
      listPatterns();
      break;
    }
    case 'h': {  // Help
      printHelp();
      break;
    }
    default:
      Serial.println("Unknown command");
  }
}

void printStatus() {
  Serial.printf("\n=== STATUS ===\n");
  Serial.printf("Band: %s\n", band_names[current_band]);
  Serial.printf("Pattern: %s\n", pattern_names[current_pattern]);
  Serial.printf("SWR: %.2f:1\n", swr_value);
  Serial.printf("Pwr Fwd: %.1f dBm\n", power_fwd_dbm);
  Serial.printf("Pwr Ref: %.1f dBm\n", power_ref_dbm);
  Serial.printf("Elements: ");
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    Serial.printf("%d ", element_state[i]);
  }
  Serial.printf("\n");
}

void listPatterns() {
  Serial.println("\n=== PATTERNS ===");
  for (int i = 0; i < NUM_PATTERNS; i++) {
    Serial.printf("%d: %s\n", i, pattern_names[i]);
  }
}

void printHelp() {
  Serial.println("\n=== COMMANDS ===");
  Serial.println("p<n>    - Set pattern (0-11)");
  Serial.println("s       - Print status");
  Serial.println("l       - List patterns");
  Serial.println("h       - This help message");
}

// ============== BLE MESH INTEGRATION (Placeholder) ==============
// Full BLE Mesh implementation requires esp-idf integration
// This skeleton shows where AFM vendor opcodes would be processed

void onAFMElementSet(uint8_t element_idx, uint8_t state) {
  setElementState(element_idx, state);
}

void onAFMPatternSet(uint8_t pattern_id) {
  if (pattern_id < NUM_PATTERNS) {
    applyPattern((BeamPattern)pattern_id);
  }
}

void onAFMBandSelect(uint8_t band_enum) {
  if (band_enum < NUM_BANDS) {
    current_band = (Band)band_enum;
  }
}
