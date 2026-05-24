/*
 * RF POWER METER - ARDUINO FIRMWARE
 * Design: Claude Code - 2025
 *
 * Wideband RF Power Meter with Digital Display
 * Frequency Range: 1.8 MHz - 1296 MHz (all ham bands)
 * Power Range: 0.1W - 1500W (with range switching)
 * Display: 16x2 LCD or 0.96" OLED
 * Detector: AD8361 True RMS or Schottky diode
 *
 * Features:
 * - Multiple display modes (W, dBm, dBW)
 * - Auto-ranging or manual range selection
 * - Peak hold function
 * - Battery voltage monitoring
 * - Calibration mode with EEPROM storage
 * - Serial output for logging
 *
 * Hardware: Arduino Nano (ATmega328P)
 *
 * Pin Connections:
 * A0 - Forward power detector input
 * A1 - Reverse power detector input (optional, for SWR)
 * A6 - Battery voltage monitor
 * D2-D5 - LCD Data (D4-D7)
 * D8 - LCD RS
 * D9 - LCD Enable
 * D10 - LCD Backlight (PWM)
 * D11 - Mode button (with pullup)
 * D12 - Range button (with pullup)
 * D13 - Status LED
 */

#include <EEPROM.h>
#include <LiquidCrystal.h>

// ===== CONFIGURATION =====

// Hardware options (comment/uncomment as needed)
#define USE_LCD              // Use 16x2 LCD display
// #define USE_OLED          // Use OLED display (requires Adafruit libraries)
#define DETECTOR_AD8361      // Using AD8361 true RMS detector
// #define DETECTOR_DIODE    // Using Schottky diode detector
#define HAS_REVERSE_DETECTOR // Second detector for SWR measurement

// Pin definitions
#define PIN_ADC_FWD    A0    // Forward power ADC input
#define PIN_ADC_REV    A1    // Reverse power ADC input
#define PIN_ADC_BATT   A6    // Battery voltage monitor
#define PIN_LCD_RS     8     // LCD Register Select
#define PIN_LCD_EN     9     // LCD Enable
#define PIN_LCD_D4     2     // LCD Data 4
#define PIN_LCD_D5     3     // LCD Data 5
#define PIN_LCD_D6     4     // LCD Data 6
#define PIN_LCD_D7     5     // LCD Data 7
#define PIN_BACKLIGHT  10    // LCD Backlight (PWM)
#define PIN_BTN_MODE   11    // Mode button
#define PIN_BTN_RANGE  12    // Range button
#define PIN_LED        13    // Status LED

// Display modes
enum DisplayMode {
  MODE_WATTS = 0,
  MODE_DBM,
  MODE_DBW,
  MODE_FWD_REV,
  MODE_SWR,
  MODE_BATTERY,
  MODE_COUNT  // Number of modes
};

// Power ranges
enum PowerRange {
  RANGE_AUTO = 0,
  RANGE_1W,
  RANGE_10W,
  RANGE_100W,
  RANGE_1000W,
  RANGE_COUNT  // Number of ranges
};

// Calibration data structure
struct CalibrationData {
  float slope;                  // Watts per volt
  float intercept;              // Offset in watts
  float freq_correction[5];     // Correction for 3.5, 7, 14, 28, 144 MHz
  uint16_t checksum;            // Data integrity check
};

// ===== GLOBAL VARIABLES =====

// LCD instance
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

// State variables
DisplayMode current_mode = MODE_WATTS;
PowerRange current_range = RANGE_AUTO;
bool peak_hold_enabled = false;
bool backlight_on = true;

// Measurement variables
float power_fwd_watts = 0.0;
float power_rev_watts = 0.0;
float power_peak_watts = 0.0;
float battery_voltage = 0.0;
float swr = 1.0;

// Calibration data
CalibrationData cal_data;

// Button debouncing
unsigned long last_mode_press = 0;
unsigned long last_range_press = 0;
unsigned long mode_hold_start = 0;
const unsigned long debounce_delay = 50;
const unsigned long long_press_time = 2000;

// Display update timing
unsigned long last_display_update = 0;
const unsigned long display_update_interval = 100;  // 10 Hz update rate

// Constants
const float ADC_REFERENCE = 5.0;      // ADC reference voltage
const int ADC_RESOLUTION = 1024;      // 10-bit ADC
const int EEPROM_CAL_ADDRESS = 0;     // EEPROM address for calibration data

// ===== SETUP =====

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println(F("RF Power Meter v1.0"));
  Serial.println(F("Design: Claude Code - 2025"));

  // Initialize pins
  pinMode(PIN_BTN_MODE, INPUT_PULLUP);
  pinMode(PIN_BTN_RANGE, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BACKLIGHT, OUTPUT);

  // Turn on backlight
  analogWrite(PIN_BACKLIGHT, 255);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("RF Power Meter"));
  lcd.setCursor(0, 1);
  lcd.print(F("Initializing..."));

  // Flash LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED, HIGH);
    delay(100);
    digitalWrite(PIN_LED, LOW);
    delay(100);
  }

  // Load calibration data from EEPROM
  loadCalibration();

  // Check if Mode button held during power-on (enter calibration mode)
  if (digitalRead(PIN_BTN_MODE) == LOW) {
    calibrationMode();
  }

  // Ready
  delay(1000);
  lcd.clear();
  Serial.println(F("Ready"));
}

// ===== MAIN LOOP =====

void loop() {
  // Read detectors
  readPowerSensors();

  // Check buttons
  checkButtons();

  // Update display if interval elapsed
  if (millis() - last_display_update >= display_update_interval) {
    updateDisplay();
    last_display_update = millis();
  }

  // Update status LED (blink when reading power)
  if (power_fwd_watts > 0.1) {
    digitalWrite(PIN_LED, (millis() / 500) % 2);  // Blink at 1 Hz
  } else {
    digitalWrite(PIN_LED, LOW);
  }

  // Serial output for logging
  if (millis() % 1000 < 10) {  // Once per second
    printSerialData();
  }
}

// ===== POWER MEASUREMENT =====

void readPowerSensors() {
  // Read forward power ADC
  int adc_fwd = analogRead(PIN_ADC_FWD);
  float voltage_fwd = (adc_fwd / (float)ADC_RESOLUTION) * ADC_REFERENCE;

  // Convert voltage to power using calibration
  power_fwd_watts = voltageToPower(voltage_fwd);

  // Apply range-specific scaling if not auto-range
  if (current_range != RANGE_AUTO) {
    power_fwd_watts = applyRangeScaling(power_fwd_watts);
  }

  // Peak hold
  if (peak_hold_enabled) {
    if (power_fwd_watts > power_peak_watts) {
      power_peak_watts = power_fwd_watts;
    }
  } else {
    power_peak_watts = power_fwd_watts;
  }

  #ifdef HAS_REVERSE_DETECTOR
  // Read reverse power ADC
  int adc_rev = analogRead(PIN_ADC_REV);
  float voltage_rev = (adc_rev / (float)ADC_RESOLUTION) * ADC_REFERENCE;
  power_rev_watts = voltageToPower(voltage_rev);

  // Calculate SWR
  if (power_fwd_watts > 0.1) {
    float reflection_coeff = sqrt(power_rev_watts / power_fwd_watts);
    if (reflection_coeff < 1.0) {
      swr = (1.0 + reflection_coeff) / (1.0 - reflection_coeff);
      if (swr > 99.9) swr = 99.9;  // Limit display
    } else {
      swr = 99.9;  // Invalid (probably bad reading)
    }
  } else {
    swr = 1.0;
  }
  #endif

  // Read battery voltage
  int adc_batt = analogRead(PIN_ADC_BATT);
  // Voltage divider: R1=10k, R2=10k, so Vbatt = Vadc * 2
  battery_voltage = (adc_batt / (float)ADC_RESOLUTION) * ADC_REFERENCE * 2.0;
}

float voltageToPower(float voltage) {
  /*
   * Convert detector voltage to power using calibration data
   *
   * For AD8361: Output is approximately logarithmic
   * Simplified linear approximation: P = slope * V + intercept
   *
   * For diode detector: More complex (square law at low levels,
   * linear at high levels), but linear approximation works
   * reasonably well over limited range with calibration
   */

  float power = cal_data.slope * voltage + cal_data.intercept;

  // Ensure non-negative
  if (power < 0.0) power = 0.0;

  return power;
}

float applyRangeScaling(float power) {
  // Apply scaling based on selected range
  // This would be used if hardware range switching affects detector sensitivity
  // For digital-only ranging, this may not be needed
  return power;
}

// ===== BUTTON HANDLING =====

void checkButtons() {
  unsigned long current_time = millis();

  // Mode button
  bool mode_pressed = (digitalRead(PIN_BTN_MODE) == LOW);
  if (mode_pressed && (current_time - last_mode_press > debounce_delay)) {
    if (mode_hold_start == 0) {
      mode_hold_start = current_time;
    } else if (current_time - mode_hold_start > long_press_time) {
      // Long press: Toggle peak hold
      peak_hold_enabled = !peak_hold_enabled;
      if (!peak_hold_enabled) {
        power_peak_watts = 0.0;  // Reset peak
      }
      mode_hold_start = 0;
      last_mode_press = current_time;

      // Feedback
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Peak Hold:"));
      lcd.setCursor(0, 1);
      lcd.print(peak_hold_enabled ? F("ON") : F("OFF"));
      delay(500);
      lcd.clear();
    }
  } else if (!mode_pressed && mode_hold_start > 0 &&
             current_time - mode_hold_start < long_press_time) {
    // Short press: Cycle display mode
    current_mode = (DisplayMode)((current_mode + 1) % MODE_COUNT);
    last_mode_press = current_time;
    mode_hold_start = 0;

    // Feedback
    lcd.clear();
  } else if (!mode_pressed) {
    mode_hold_start = 0;
  }

  // Range button
  if (digitalRead(PIN_BTN_RANGE) == LOW &&
      (current_time - last_range_press > debounce_delay)) {
    // Short press: Cycle range
    current_range = (PowerRange)((current_range + 1) % RANGE_COUNT);
    if (peak_hold_enabled && current_range != RANGE_AUTO) {
      power_peak_watts = 0.0;  // Reset peak on range change
    }
    last_range_press = current_time;

    // Feedback
    lcd.clear();
  }
}

// ===== DISPLAY =====

void updateDisplay() {
  lcd.setCursor(0, 0);

  // First line: Range and peak indicator
  displayRange();
  if (peak_hold_enabled) {
    lcd.print(F("  PEAK"));
  }

  lcd.setCursor(0, 1);

  // Second line: Measurement based on mode
  switch (current_mode) {
    case MODE_WATTS:
      displayWatts();
      break;

    case MODE_DBM:
      displayDBm();
      break;

    case MODE_DBW:
      displayDBW();
      break;

    case MODE_FWD_REV:
      displayForwardReverse();
      break;

    case MODE_SWR:
      displaySWR();
      break;

    case MODE_BATTERY:
      displayBattery();
      break;
  }

  // Clear rest of line
  for (int i = lcd.getCursor(); i < 16; i++) {
    lcd.print(F(" "));
  }
}

void displayRange() {
  switch (current_range) {
    case RANGE_AUTO:
      lcd.print(F("AUTO "));
      break;
    case RANGE_1W:
      lcd.print(F("1W   "));
      break;
    case RANGE_10W:
      lcd.print(F("10W  "));
      break;
    case RANGE_100W:
      lcd.print(F("100W "));
      break;
    case RANGE_1000W:
      lcd.print(F("1kW  "));
      break;
  }
}

void displayWatts() {
  float power = peak_hold_enabled ? power_peak_watts : power_fwd_watts;

  // Format based on magnitude
  if (power < 1.0) {
    lcd.print(F("  "));
    lcd.print(power, 2);  // Two decimal places
    lcd.print(F(" W"));
  } else if (power < 10.0) {
    lcd.print(F(" "));
    lcd.print(power, 1);  // One decimal place
    lcd.print(F(" W"));
  } else if (power < 1000.0) {
    lcd.print(F("  "));
    lcd.print((int)power);  // No decimal
    lcd.print(F(" W"));
  } else {
    lcd.print((int)power);
    lcd.print(F(" W"));
  }
}

void displayDBm() {
  float power = peak_hold_enabled ? power_peak_watts : power_fwd_watts;
  float dbm = 10.0 * log10(power * 1000.0);  // dBm = 10*log10(P_mW)

  if (dbm >= 0) {
    lcd.print(F(" +"));
  } else {
    lcd.print(F("  "));
  }
  lcd.print(dbm, 1);
  lcd.print(F(" dBm"));
}

void displayDBW() {
  float power = peak_hold_enabled ? power_peak_watts : power_fwd_watts;
  float dbw = 10.0 * log10(power);  // dBW = 10*log10(P_W)

  if (dbw >= 0) {
    lcd.print(F(" +"));
  } else {
    lcd.print(F("  "));
  }
  lcd.print(dbw, 1);
  lcd.print(F(" dBW"));
}

void displayForwardReverse() {
  #ifdef HAS_REVERSE_DETECTOR
  // Show both forward and reverse on one line (compact)
  lcd.print(F("F:"));
  lcd.print(power_fwd_watts, 1);
  lcd.print(F("W R:"));
  lcd.print(power_rev_watts, 1);
  lcd.print(F("W"));
  #else
  lcd.print(F("No Rev Detector"));
  #endif
}

void displaySWR() {
  #ifdef HAS_REVERSE_DETECTOR
  lcd.print(F("SWR: "));
  if (swr < 10.0) {
    lcd.print(swr, 2);
  } else {
    lcd.print(swr, 1);
  }
  lcd.print(F(":1"));
  #else
  lcd.print(F("No Rev Detector"));
  #endif
}

void displayBattery() {
  lcd.print(F("Batt: "));
  lcd.print(battery_voltage, 1);
  lcd.print(F("V"));

  // Battery percentage (9V nominal)
  int percent = (int)((battery_voltage - 7.0) / 2.0 * 100.0);
  if (percent > 100) percent = 100;
  if (percent < 0) percent = 0;

  lcd.setCursor(0, 1);
  lcd.print(F("Capacity: "));
  lcd.print(percent);
  lcd.print(F("%"));
}

// ===== SERIAL OUTPUT =====

void printSerialData() {
  Serial.print(F("FWD: "));
  Serial.print(power_fwd_watts, 2);
  Serial.print(F(" W"));

  #ifdef HAS_REVERSE_DETECTOR
  Serial.print(F("  REV: "));
  Serial.print(power_rev_watts, 2);
  Serial.print(F(" W"));
  Serial.print(F("  SWR: "));
  Serial.print(swr, 2);
  Serial.print(F(":1"));
  #endif

  Serial.print(F("  BATT: "));
  Serial.print(battery_voltage, 1);
  Serial.println(F(" V"));
}

// ===== CALIBRATION =====

void calibrationMode() {
  lcd.clear();
  lcd.print(F("Calibration Mode"));
  delay(1000);

  int cal_point = 0;
  float cal_powers[] = {1.0, 10.0, 50.0, 100.0, 500.0};
  float cal_voltages[5];
  int num_points = 5;

  for (cal_point = 0; cal_point < num_points; cal_point++) {
    lcd.clear();
    lcd.print(F("Apply "));
    lcd.print(cal_powers[cal_point], 0);
    lcd.print(F(" W"));

    // Wait for user to apply power and press mode button
    while (digitalRead(PIN_BTN_MODE) == HIGH) {
      // Read and display current voltage
      int adc_val = analogRead(PIN_ADC_FWD);
      float voltage = (adc_val / (float)ADC_RESOLUTION) * ADC_REFERENCE;

      lcd.setCursor(0, 1);
      lcd.print(F("V: "));
      lcd.print(voltage, 3);
      lcd.print(F(" V  "));

      delay(100);
    }

    // Store voltage reading
    int adc_val = analogRead(PIN_ADC_FWD);
    cal_voltages[cal_point] = (adc_val / (float)ADC_RESOLUTION) * ADC_REFERENCE;

    // Wait for button release
    while (digitalRead(PIN_BTN_MODE) == LOW) {
      delay(10);
    }
    delay(200);  // Debounce
  }

  // Calculate calibration using linear regression
  calculateCalibration(cal_powers, cal_voltages, num_points);

  // Save to EEPROM
  saveCalibration();

  lcd.clear();
  lcd.print(F("Cal Complete!"));
  lcd.setCursor(0, 1);
  lcd.print(F("Slope: "));
  lcd.print(cal_data.slope, 1);
  delay(3000);

  // Reset and restart
  lcd.clear();
  lcd.print(F("Restarting..."));
  delay(1000);
  asm volatile ("  jmp 0");  // Software reset
}

void calculateCalibration(float* powers, float* voltages, int n) {
  /*
   * Simple linear regression: P = slope * V + intercept
   *
   * slope = (n*Σ(V*P) - Σ(V)*Σ(P)) / (n*Σ(V²) - (Σ(V))²)
   * intercept = (Σ(P) - slope*Σ(V)) / n
   */

  float sum_v = 0, sum_p = 0, sum_vp = 0, sum_v2 = 0;

  for (int i = 0; i < n; i++) {
    sum_v += voltages[i];
    sum_p += powers[i];
    sum_vp += voltages[i] * powers[i];
    sum_v2 += voltages[i] * voltages[i];
  }

  cal_data.slope = (n * sum_vp - sum_v * sum_p) / (n * sum_v2 - sum_v * sum_v);
  cal_data.intercept = (sum_p - cal_data.slope * sum_v) / n;

  // Initialize frequency correction to 1.0 (no correction)
  for (int i = 0; i < 5; i++) {
    cal_data.freq_correction[i] = 1.0;
  }
}

void loadCalibration() {
  EEPROM.get(EEPROM_CAL_ADDRESS, cal_data);

  // Calculate checksum
  uint16_t checksum = calculateChecksum();

  if (cal_data.checksum != checksum) {
    // Invalid calibration data, use defaults
    Serial.println(F("Invalid calibration, using defaults"));
    cal_data.slope = 50.0;         // Default for AD8361 with -30dB coupling
    cal_data.intercept = -2.0;     // Default offset
    for (int i = 0; i < 5; i++) {
      cal_data.freq_correction[i] = 1.0;
    }
    saveCalibration();
  } else {
    Serial.println(F("Calibration loaded from EEPROM"));
  }

  Serial.print(F("Slope: "));
  Serial.print(cal_data.slope);
  Serial.print(F("  Intercept: "));
  Serial.println(cal_data.intercept);
}

void saveCalibration() {
  cal_data.checksum = calculateChecksum();
  EEPROM.put(EEPROM_CAL_ADDRESS, cal_data);
  Serial.println(F("Calibration saved to EEPROM"));
}

uint16_t calculateChecksum() {
  // Simple checksum: sum of all bytes except checksum field
  uint16_t sum = 0;
  uint8_t* ptr = (uint8_t*)&cal_data;
  for (int i = 0; i < sizeof(CalibrationData) - sizeof(uint16_t); i++) {
    sum += ptr[i];
  }
  return sum;
}

// ===== END OF FIRMWARE =====
