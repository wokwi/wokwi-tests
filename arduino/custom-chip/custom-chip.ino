#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  if (!Wire.requestFrom(0x22, 1)) {
    Serial.println("Chip not responding. Did you create i2c-counter.chip.c?");
    delay(500);
  } else {
    Serial.print("Chip value: ");
    uint8_t value = Wire.read();
    Serial.println(value);
    delay(10);
  }
}
