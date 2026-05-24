// INTEGRATED ANTENNA ANALYZER - ESP32 FIRMWARE
// Vector network analyzer covering all ham bands 160M-20cm (1.8MHz-1.3GHz)
// Phase measurement via AD8302, dual synthesizers (AD9851 + ADF4351)
// TFT display, Bluetooth/WiFi logging, SD card data storage

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SD.h>
#include <Preferences.h>
#include <BluetoothSerial.h>

// Hardware version and constants
#define FW_VERSION "1.0.0"
#define NUM_BANDS 16
#define NUM_SWEEP_POINTS 100
#define CALIBRATION_POINTS 50

// ============ GPIO PIN ASSIGNMENTS ============

// HF Synthesizer (AD9851 DDS)
#define AD9851_D0 32
#define AD9851_D1 33
#define AD9851_D2 25
#define AD9851_D3 26
#define AD9851_D4 27
#define AD9851_D5 14
#define AD9851_D6 12
#define AD9851_D7 13
#define AD9851_W_CLK 5
#define AD9851_FQ_UD 17
#define AD9851_RST 16

// VHF/UHF Synthesizer (ADF4351 PLL)
#define ADF4351_CLK 18
#define ADF4351_DATA 19
#define ADF4351_LE 21
#define ADF4351_CE 22

// RF Band Selection
#define HF_EN GPIO_NUM_0        // AD9851 enable
#define VHF_UHF_EN GPIO_NUM_10  // ADF4351 enable
#define HF_FILTER GPIO_NUM_1    // Low-pass filter enable
#define VHF_FILTER GPIO_NUM_10  // Band-pass filter (shared with ADF4351)
#define UHF_FILTER GPIO_NUM_11  // High-pass filter enable
#define COUPLER_SEL GPIO_NUM_11 // HF/VHF vs UHF coupler select

// Front-End Protection
#define MUTE_RF GPIO_NUM_12     // Absorptive switch control
#define GPIO_OVR_DETECT GPIO_NUM_35  // Overvoltage ADC input

// Display & Control
#define ROTARY_CLK GPIO_NUM_34
#define ROTARY_DT GPIO_NUM_35
#define ROTARY_SW GPIO_NUM_36
#define STATUS_LED GPIO_NUM_2
#define TFT_CS GPIO_NUM_4
#define TFT_DC GPIO_NUM_2
#define TFT_RST GPIO_NUM_15

// Battery & Power
#define BATT_ADC GPIO_NUM_36

// ============ DATA STRUCTURES ============

struct BandConfig {
  const char *name;           // "160M", "80M", etc.
  uint32_t fmin_hz;
  uint32_t fmax_hz;
  uint32_t fstep_hz;
  uint8_t synthesizer;        // 0=AD9851, 1=ADF4351
  uint8_t coupler;            // 0=HF toroid, 1=VHF stripline, 2=UHF microstrip
};

struct SweepPoint {
  uint32_t freq_hz;
  float vmag;                 // AD8302 VMAG output (V)
  float vphs;                 // AD8302 VPHS output (V)
  float gain_db;              // Gain in dB
  float phase_deg;            // Phase in degrees
  float gamma_r, gamma_i;     // Reflection coefficient (complex)
  float z_real, z_imag;       // Impedance (Ω)
  float vswr;
  float return_loss_db;
};

struct CalibrationData {
  float open_mag[CALIBRATION_POINTS];
  float open_phs[CALIBRATION_POINTS];
  float short_mag[CALIBRATION_POINTS];
  float short_phs[CALIBRATION_POINTS];
  float load_mag[CALIBRATION_POINTS];
  float load_phs[CALIBRATION_POINTS];
  uint32_t cal_timestamp;
  uint8_t valid;
};

// ============ GLOBAL VARIABLES ============

Adafruit_ADS1115 ads;
TFT_eSPI tft = TFT_eSPI();
BluetoothSerial SerialBT;
Preferences preferences;
CalibrationData calibration;
SweepPoint sweep_data[NUM_SWEEP_POINTS];
uint8_t current_band = 0;
uint8_t current_coupler = 0;
float battery_voltage = 9.6;
bool measurement_active = false;
bool calibration_valid = false;

// Ham band frequency table (16 bands covering 1.8MHz-1.3GHz)
BandConfig bands[NUM_BANDS] = {
  {"160M", 1800000, 2000000, 1000, 0, 0},        // 0: HF, FT50-43
  {"80M", 3500000, 4000000, 1000, 0, 0},         // 1
  {"40M", 7000000, 7300000, 1000, 0, 0},         // 2
  {"30M", 10100000, 10150000, 500, 0, 0},        // 3
  {"20M", 14000000, 14350000, 1000, 0, 0},       // 4
  {"17M", 18068000, 18168000, 500, 0, 0},        // 5
  {"15M", 21000000, 21450000, 1000, 0, 0},       // 6
  {"12M", 24890000, 24990000, 500, 0, 0},        // 7
  {"10M", 28000000, 29700000, 1000, 0, 0},       // 8
  {"6M", 50000000, 54000000, 10000, 1, 1},       // 9: VHF, stripline, ADF4351
  {"2M", 144000000, 148000000, 10000, 1, 1},     // 10
  {"1.25M", 222000000, 225000000, 10000, 1, 1},  // 11
  {"70cm", 430000000, 450000000, 100000, 1, 2},  // 12: UHF, microstrip, ADF4351
  {"33cm", 902000000, 928000000, 100000, 1, 2},  // 13
  {"20cm", 1296000000, 1300000000, 100000, 1, 2},// 14
  {"23cm", 1240000000, 1300000000, 100000, 1, 2} // 15 (alternate)
};

// ============ SETUP & INITIALIZATION ============

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\n=== INTEGRATED ANTENNA ANALYZER ===");
  Serial.printf("Firmware v%s\n", FW_VERSION);

  // Initialize all GPIO pins
  initGPIOs();

  // Initialize I2C for ADS1115
  Wire.begin(21, 22);  // SDA=GPIO21, SCL=GPIO22
  if (!ads.begin(0x48)) {
    Serial.println("ERROR: ADS1115 not found!");
    while (1);
  }
  ads.setGain(GAIN_ONE);

  // Initialize TFT display
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("Initializing...");

  // Initialize SD card
  if (!SD.begin(5)) {  // GPIO5 CS pin
    Serial.println("SD card mount failed!");
  } else {
    Serial.println("SD card initialized OK");
  }

  // Load calibration data
  loadCalibration();

  // Initialize Bluetooth
  if (!SerialBT.begin("ANT_ANALYZER")) {
    Serial.println("Bluetooth init failed!");
  }

  // Initial frequency (40M band, 7.1 MHz)
  selectBand(2);
  setFrequency(7100000);

  tft.fillScreen(TFT_BLACK);
  displaySplashScreen();
  delay(2000);

  Serial.println("Initialization complete!");
}

void loop() {
  // Main event loop
  checkBatteryVoltage();
  updateDisplay();
  handleRotaryEncoder();
  handleBluetoothCommands();
  checkOvervoltage();

  delay(50);
}

// ============ GPIO INITIALIZATION ============

void initGPIOs() {
  // AD9851 data bus
  pinMode(AD9851_D0, OUTPUT);
  pinMode(AD9851_D1, OUTPUT);
  pinMode(AD9851_D2, OUTPUT);
  pinMode(AD9851_D3, OUTPUT);
  pinMode(AD9851_D4, OUTPUT);
  pinMode(AD9851_D5, OUTPUT);
  pinMode(AD9851_D6, OUTPUT);
  pinMode(AD9851_D7, OUTPUT);
  pinMode(AD9851_W_CLK, OUTPUT);
  pinMode(AD9851_FQ_UD, OUTPUT);
  pinMode(AD9851_RST, OUTPUT);

  // ADF4351 SPI
  pinMode(ADF4351_CLK, OUTPUT);
  pinMode(ADF4351_DATA, OUTPUT);
  pinMode(ADF4351_LE, OUTPUT);
  pinMode(ADF4351_CE, OUTPUT);

  // Band selection
  pinMode(HF_EN, OUTPUT);
  pinMode(VHF_UHF_EN, OUTPUT);
  pinMode(HF_FILTER, OUTPUT);
  pinMode(VHF_FILTER, OUTPUT);
  pinMode(UHF_FILTER, OUTPUT);
  pinMode(COUPLER_SEL, OUTPUT);

  // Protection
  pinMode(MUTE_RF, OUTPUT);
  digitalWrite(MUTE_RF, LOW);  // Start muted

  // Control
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);
  pinMode(ROTARY_SW, INPUT_PULLUP);
  pinMode(STATUS_LED, OUTPUT);

  // Battery monitoring
  pinMode(BATT_ADC, INPUT);

  // Set initial states
  digitalWrite(HF_EN, LOW);
  digitalWrite(VHF_UHF_EN, LOW);
  digitalWrite(HF_FILTER, LOW);
  digitalWrite(VHF_FILTER, LOW);
  digitalWrite(UHF_FILTER, LOW);
}

// ============ AD9851 DDS CONTROL ============

void setAD9851Frequency(uint32_t ftw) {
  // Load 32-bit FTW (frequency tuning word) across 4 parallel bytes

  uint8_t bytes[4] = {
    (ftw >> 24) & 0xFF,
    (ftw >> 16) & 0xFF,
    (ftw >> 8) & 0xFF,
    ftw & 0xFF
  };

  for (int byte_idx = 0; byte_idx < 4; byte_idx++) {
    uint8_t byte_val = bytes[byte_idx];

    // Set D0-D7 bits from byte
    digitalWrite(AD9851_D0, (byte_val >> 0) & 1);
    digitalWrite(AD9851_D1, (byte_val >> 1) & 1);
    digitalWrite(AD9851_D2, (byte_val >> 2) & 1);
    digitalWrite(AD9851_D3, (byte_val >> 3) & 1);
    digitalWrite(AD9851_D4, (byte_val >> 4) & 1);
    digitalWrite(AD9851_D5, (byte_val >> 5) & 1);
    digitalWrite(AD9851_D6, (byte_val >> 6) & 1);
    digitalWrite(AD9851_D7, (byte_val >> 7) & 1);

    // Clock strobe
    digitalWrite(AD9851_W_CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(AD9851_W_CLK, LOW);
  }

  // Frequency update strobe
  digitalWrite(AD9851_FQ_UD, HIGH);
  delayMicroseconds(1);
  digitalWrite(AD9851_FQ_UD, LOW);

  delay(1);
}

uint32_t frequencyToFTW(uint32_t freq_hz) {
  // Convert frequency to AD9851 FTW
  // FTW = freq × 2^32 / (33.333MHz × 6) = freq × 21.47483648
  return (uint32_t)((freq_hz * 21.47483648) + 0.5);
}

// ============ ADF4351 PLL CONTROL ============

void setADF4351Frequency(uint32_t freq_hz) {
  // ADF4351: f = (INT + FRAC/2^25) × (REFIN / R)
  // REFIN = 25 MHz, R = 1, so f = (INT + FRAC/2^25) × 25MHz

  uint32_t int_part = freq_hz / 25000000;
  uint32_t frac_part = ((freq_hz % 25000000) << 25) / 25000000;

  // Register values
  uint32_t r0 = (frac_part << 14) | (int_part << 2) | 0x0;
  uint32_t r1 = (1 << 23) | 0x00008001;  // Prescaler 8/9
  uint32_t r2 = (0x05 << 22) | 0x00000000;  // CP=2.5mA
  uint32_t r3 = (0x02 << 2) | (1 << 1) | 0x01;  // +2dBm, ON
  uint32_t r4 = 0x00000004;
  uint32_t r5 = 0x00800005;

  // Write registers (R5 down to R0)
  writeSPIRegister(r5);
  delayMicroseconds(100);
  writeSPIRegister(r4);
  delayMicroseconds(100);
  writeSPIRegister(r3);
  delayMicroseconds(100);
  writeSPIRegister(r2);
  delayMicroseconds(100);
  writeSPIRegister(r1);
  delayMicroseconds(100);
  writeSPIRegister(r0);
  delayMicroseconds(100);

  delay(10);
}

void writeSPIRegister(uint32_t value) {
  // 32-bit MSB-first SPI write to ADF4351
  for (int i = 31; i >= 0; i--) {
    digitalWrite(ADF4351_CLK, LOW);
    digitalWrite(ADF4351_DATA, (value >> i) & 1);
    digitalWrite(ADF4351_CLK, HIGH);
  }
  digitalWrite(ADF4351_LE, HIGH);
  delayMicroseconds(10);
  digitalWrite(ADF4351_LE, LOW);
}

// ============ FREQUENCY & BAND SELECTION ============

void selectBand(uint8_t band_num) {
  if (band_num >= NUM_BANDS) return;

  current_band = band_num;
  BandConfig *band = &bands[band_num];

  // Select synthesizer
  if (band->synthesizer == 0) {
    // AD9851 (HF)
    digitalWrite(HF_EN, HIGH);
    digitalWrite(VHF_UHF_EN, LOW);
  } else {
    // ADF4351 (VHF/UHF)
    digitalWrite(HF_EN, LOW);
    digitalWrite(VHF_UHF_EN, HIGH);
  }

  // Select coupler and filter
  selectCoupler(band->fmin_hz);
  selectBandFilter(band->fmin_hz);

  Serial.printf("Band selected: %s (%.3f - %.3f MHz)\n",
      band->name, band->fmin_hz / 1e6, band->fmax_hz / 1e6);
}

void setFrequency(uint32_t freq_hz) {
  BandConfig *band = &bands[current_band];

  if (band->synthesizer == 0) {
    // AD9851
    uint32_t ftw = frequencyToFTW(freq_hz);
    setAD9851Frequency(ftw);
  } else {
    // ADF4351
    setADF4351Frequency(freq_hz);
  }

  delay(2);  // Settling time
}

void selectCoupler(uint32_t freq_hz) {
  if (freq_hz < 400e6) {
    // HF/VHF coupler
    digitalWrite(COUPLER_SEL, LOW);
    current_coupler = 0;
  } else {
    // UHF coupler
    digitalWrite(COUPLER_SEL, HIGH);
    current_coupler = 1;
  }
}

void selectBandFilter(uint32_t freq_hz) {
  // Only one filter enabled at a time

  digitalWrite(HF_FILTER, LOW);
  digitalWrite(VHF_FILTER, LOW);
  digitalWrite(UHF_FILTER, LOW);

  if (freq_hz < 70e6) {
    digitalWrite(HF_FILTER, HIGH);
  } else if (freq_hz < 400e6) {
    digitalWrite(VHF_FILTER, HIGH);
  } else {
    digitalWrite(UHF_FILTER, HIGH);
  }

  delay(2);
}

// ============ MEASUREMENT & CALIBRATION ============

void readAD8302(float &vmag, float &vphs) {
  // Read VMAG and VPHS from AD8302 via ADS1115
  int16_t mag_raw = ads.readADC_SingleEnded(0);  // AIN0
  int16_t phs_raw = ads.readADC_SingleEnded(1);  // AIN1

  vmag = mag_raw * 0.1875 / 1000.0;  // Convert to volts (0.1875mV per count)
  vphs = phs_raw * 0.1875 / 1000.0;

  // Clamp values to safe range
  if (vmag > 3.2) vmag = 3.2;
  if (vphs > 1.8) vphs = 1.8;
  if (vmag < 0.6) vmag = 0.6;
  if (vphs < 0.0) vphs = 0.0;
}

void voltageToMagnitudePhase(float vmag, float vphs, float &gain_db, float &phase_deg) {
  // Convert AD8302 output voltages to dB and degrees
  // VMAG: 30 mV/dB, range 0.6-3.2V = -60 to 0 dBm
  // VPHS: 10 mV/degree, range 0-1.8V = 0-180°

  gain_db = (vmag * 1000.0 - 600.0) / 30.0 - 60.0;
  phase_deg = (vphs * 1000.0 - 900.0) / 10.0;  // 0.9V = 0°
}

void frequencySweep(uint32_t fmin, uint32_t fmax, uint32_t fstep) {
  // Perform frequency sweep, collect S11 data
  uint32_t num_points = 0;

  Serial.printf("Sweep: %.3f - %.3f MHz\n", fmin / 1e6, fmax / 1e6);
  digitalWrite(MUTE_RF, HIGH);  // Unmute RF
  delay(10);

  for (uint32_t freq = fmin; freq <= fmax && num_points < NUM_SWEEP_POINTS; freq += fstep) {
    setFrequency(freq);
    delay(5);  // Settling

    float vmag, vphs;
    readAD8302(vmag, vphs);

    float gain_db, phase_deg;
    voltageToMagnitudePhase(vmag, vphs, gain_db, phase_deg);

    // Store data
    SweepPoint *pt = &sweep_data[num_points];
    pt->freq_hz = freq;
    pt->vmag = vmag;
    pt->vphs = vphs;
    pt->gain_db = gain_db;
    pt->phase_deg = phase_deg;

    // Convert to impedance (simplified - requires calibration data)
    gainPhaseToImpedance(gain_db, phase_deg, pt->z_real, pt->z_imag, pt->gamma_r, pt->gamma_i);

    // Calculate VSWR
    float mag_gamma = sqrt(pt->gamma_r * pt->gamma_r + pt->gamma_i * pt->gamma_i);
    pt->vswr = (1.0 + mag_gamma) / (1.0 - mag_gamma);
    pt->return_loss_db = -20.0 * log10(mag_gamma + 1e-10);

    num_points++;

    Serial.printf("  %.3f MHz: %+6.2f dB, %+7.1f°, Z=%6.1f+j%6.1f\n",
        freq / 1e6, gain_db, phase_deg, pt->z_real, pt->z_imag);

    delay(10);
  }

  digitalWrite(MUTE_RF, LOW);  // Mute RF after measurement
  Serial.printf("Sweep complete: %d points\n", num_points);
}

void gainPhaseToImpedance(float gain_db, float phase_deg, float &z_r, float &z_i, float &gr, float &gi) {
  // Convert magnitude and phase to impedance (simplified)
  // Requires proper calibration with open/short/load standards

  // Magnitude in linear form
  float mag = pow(10.0, gain_db / 20.0);

  // Reflection coefficient (complex)
  float phase_rad = phase_deg * M_PI / 180.0;
  gr = mag * cos(phase_rad);
  gi = mag * sin(phase_rad);

  // Impedance from reflection coefficient: Z = 50 × (1+Γ)/(1-Γ)
  float denom_r = 1.0 - gr;
  float denom_i = -gi;
  float denom_mag2 = denom_r * denom_r + denom_i * denom_i;

  float numer_r = (1.0 + gr) * denom_r - gi * denom_i;
  float numer_i = (1.0 + gr) * denom_i + gi * denom_r;

  z_r = 50.0 * numer_r / denom_mag2;
  z_i = 50.0 * numer_i / denom_mag2;
}

void saveCalibration() {
  // Save OSL calibration data to NVS (SPIFFS alternative)
  preferences.begin("calibration", false);
  preferences.putBytes("open_mag", (uint8_t*)calibration.open_mag, sizeof(calibration.open_mag));
  preferences.putBytes("open_phs", (uint8_t*)calibration.open_phs, sizeof(calibration.open_phs));
  preferences.putBytes("short_mag", (uint8_t*)calibration.short_mag, sizeof(calibration.short_mag));
  preferences.putBytes("short_phs", (uint8_t*)calibration.short_phs, sizeof(calibration.short_phs));
  preferences.putBytes("load_mag", (uint8_t*)calibration.load_mag, sizeof(calibration.load_mag));
  preferences.putBytes("load_phs", (uint8_t*)calibration.load_phs, sizeof(calibration.load_phs));
  preferences.putULong("timestamp", calibration.cal_timestamp);
  preferences.putUChar("valid", calibration.valid);
  preferences.end();

  Serial.println("Calibration saved");
}

void loadCalibration() {
  // Load OSL calibration from NVS
  preferences.begin("calibration", true);
  preferences.getBytes("open_mag", (uint8_t*)calibration.open_mag, sizeof(calibration.open_mag));
  preferences.getBytes("open_phs", (uint8_t*)calibration.open_phs, sizeof(calibration.open_phs));
  preferences.getBytes("short_mag", (uint8_t*)calibration.short_mag, sizeof(calibration.short_mag));
  preferences.getBytes("short_phs", (uint8_t*)calibration.short_phs, sizeof(calibration.short_phs));
  preferences.getBytes("load_mag", (uint8_t*)calibration.load_mag, sizeof(calibration.load_mag));
  preferences.getBytes("load_phs", (uint8_t*)calibration.load_phs, sizeof(calibration.load_phs));
  calibration.cal_timestamp = preferences.getULong("timestamp", 0);
  calibration.valid = preferences.getUChar("valid", 0);
  preferences.end();

  calibration_valid = (calibration.valid == 1);
  Serial.printf("Calibration loaded: %s\n", calibration_valid ? "VALID" : "INVALID");
}

// ============ DISPLAY ============

void displaySplashScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(30, 50);
  tft.println("ANT ANALYZER");
  tft.setCursor(40, 80);
  tft.println("v1.0.0");
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 130);
  tft.println("All Bands 160M-20cm");
  tft.setCursor(10, 150);
  tft.println("Vector VNA Mode");
}

void updateDisplay() {
  static unsigned long last_update = 0;
  if (millis() - last_update < 500) return;  // Update every 500ms
  last_update = millis();

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);

  // Band and frequency
  BandConfig *band = &bands[current_band];
  tft.setCursor(0, 0);
  tft.printf("%s: %d - %d MHz\n", band->name, band->fmin_hz / 1e6, band->fmax_hz / 1e6);

  // Battery voltage
  tft.printf("Battery: %.2f V\n", battery_voltage);

  // Calibration status
  tft.setTextColor(calibration_valid ? TFT_GREEN : TFT_RED);
  tft.printf("Cal: %s\n", calibration_valid ? "VALID" : "NONE");

  tft.setTextColor(TFT_WHITE);
  tft.printf("Synth: %s\n", band->synthesizer == 0 ? "AD9851" : "ADF4351");

  // Instructions
  tft.setCursor(0, 200);
  tft.printf("Encoder: Select Band\nPress: Sweep\n");
}

// ============ ROTARY ENCODER & CONTROLS ============

void handleRotaryEncoder() {
  static uint8_t last_clk = 1;
  static unsigned long last_change = 0;

  uint8_t clk = digitalRead(ROTARY_CLK);
  uint8_t dt = digitalRead(ROTARY_DT);

  if (clk != last_clk) {
    if (millis() - last_change < 20) return;  // Debounce
    last_change = millis();

    if (clk == 0 && dt == 1) {
      // Clockwise
      current_band = (current_band + 1) % NUM_BANDS;
    } else if (clk == 0 && dt == 0) {
      // Counter-clockwise
      current_band = (current_band > 0) ? (current_band - 1) : (NUM_BANDS - 1);
    }
    selectBand(current_band);
  }

  last_clk = clk;

  // Button press: Start sweep
  if (digitalRead(ROTARY_SW) == 0) {
    static unsigned long last_press = 0;
    if (millis() - last_press > 500) {
      last_press = millis();
      BandConfig *band = &bands[current_band];
      frequencySweep(band->fmin_hz, band->fmax_hz, band->fstep_hz);
    }
  }
}

// ============ BLUETOOTH & LOGGING ============

void handleBluetoothCommands() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();

    if (cmd == "SWEEP") {
      BandConfig *band = &bands[current_band];
      frequencySweep(band->fmin_hz, band->fmax_hz, band->fstep_hz);
    } else if (cmd.startsWith("FREQ ")) {
      uint32_t freq = cmd.substring(5).toInt();
      setFrequency(freq * 1000);  // Convert MHz to Hz
    } else if (cmd.startsWith("BAND ")) {
      uint8_t band_num = cmd.substring(5).toInt();
      selectBand(band_num);
    }
  }
}

// ============ BATTERY & PROTECTION ============

void checkBatteryVoltage() {
  static unsigned long last_check = 0;
  if (millis() - last_check < 5000) return;  // Check every 5 seconds
  last_check = millis();

  int raw = analogRead(BATT_ADC);
  battery_voltage = (raw / 4095.0) * 3.3 * 5.8;  // Convert to battery voltage

  if (battery_voltage < 7.5) {
    Serial.println("BATTERY CRITICAL - SHUTDOWN");
    digitalWrite(MUTE_RF, LOW);
    tft.fillScreen(TFT_RED);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 100);
    tft.println("BATTERY LOW");
    delay(5000);
    esp_deep_sleep_start();  // Deep sleep
  }
}

void checkOvervoltage() {
  float vmag, vphs;
  readAD8302(vmag, vphs);

  if (vmag > 3.3 || vphs > 1.9) {
    Serial.println("OVERVOLTAGE DETECTED - MUTING");
    digitalWrite(MUTE_RF, LOW);
    measurement_active = false;
  }
}

// ============ END OF FIRMWARE ============
