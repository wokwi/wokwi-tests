void setup() {
  // Wait for USB Serial to become available
  while (!Serial) {
    delay(1);
  }
  
  Serial.begin(115200);
  String name = Serial.readStringUntil('\n');
  Serial.print("Hello, ");
  Serial.print(name);
  Serial.println("!");
}

void loop() {
}
