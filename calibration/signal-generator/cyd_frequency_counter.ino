// ESP32/CYD simple pulse counter for GPS 1PPS or divided RF.
// Displays counts per second over Serial. Adapt display code as needed.

volatile uint32_t pulse_count = 0;

void IRAM_ATTR onPulse() {
  pulse_count++;
}

void setup() {
  Serial.begin(115200);
  pinMode(25, INPUT_PULLUP); // Connect PPS or divider output here
  attachInterrupt(digitalPinToInterrupt(25), onPulse, RISING);
}

void loop() {
  uint32_t start = millis();
  pulse_count = 0;
  while (millis() - start < 1000) {
    // count for 1 second
    delay(1);
  }
  Serial.print("Pulses per second: ");
  Serial.println(pulse_count);
  delay(500);
}
