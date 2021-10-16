#include <SD.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    Serial.flush();
    asm("BREAK");
  }

  File root = SD.open("/");
  printDirectory(root);
  Serial.println("done!");
  Serial.flush();
}

void loop() { }

void printDirectory(File dir) {
  File entry = dir.openNextFile();
  while (entry) {
    Serial.println(entry.name());
    entry.close();
    entry = dir.openNextFile();
  }
}
