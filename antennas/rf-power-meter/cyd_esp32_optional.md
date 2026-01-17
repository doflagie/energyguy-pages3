# Optional ESP32/CYD Helper

Some RF power meter calibrations are easier with a Cheap Yellow Display (ESP32-2432S028R). This sketch counts pulses and shows a stable readout for comparisons.

```cpp
#include <Arduino.h>

const int inputPin = 34;  // frequency input
volatile uint32_t edges = 0;

void IRAM_ATTR onEdge() {
  edges++;
}

void setup() {
  Serial.begin(115200);
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), onEdge, RISING);
}

void loop() {
  edges = 0;
  uint32_t start = millis();
  delay(1000);
  uint32_t elapsed = millis() - start;
  uint32_t hz = (edges * 1000UL) / (elapsed ? elapsed : 1);
  Serial.print("Freq: ");
  Serial.print(hz);
  Serial.println(" Hz");
}
```
