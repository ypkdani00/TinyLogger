#include <Arduino.h>
#include <TinyLogger.h>

void setup() {
  Serial.begin(115200);
  Log.begin(&Serial, TinyLogger::Level::VERBOSE);
}

unsigned long prevReport = 0;
void loop() {
  if (millis() - prevReport > 1000) {
    Log.infoln("Loop");
    
    Log.sfatalln("MAINLOOP", "Test fatal msg");
    Log.serrorln("MAINLOOP", "Test error msg");
    Log.swarningln("MAINLOOP", "Test warning msg");
    Log.sinfoln("MAINLOOP", "Test info msg");
    Log.snoticeln("MAINLOOP", "Test notice msg");

#ifdef ESP32
    Log.straceln("MAINLOOP", "Esp32 free heap: %d", ESP.getFreeHeap());
#endif

#ifdef ESP8266
    Log.straceln("MAINLOOP", "Esp8266 free heap: %d", ESP.getFreeHeap());
#endif

    Log.verboseln("After start: %d ms.", millis());

    prevReport = millis();
  }
}