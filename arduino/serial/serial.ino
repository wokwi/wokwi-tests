void setup() {
  Serial.begin(115200);
  String name = Serial.readStringUntil('\n');
  Serial.print("Hello, ");
  Serial.print(name);
  Serial.println("!");
}

void loop() {
}
