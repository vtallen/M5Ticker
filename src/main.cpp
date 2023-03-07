#include <Arduino.h>
#include <M5Core2.h>

#include <helpers/wifi_helper.h>
#include <ticker_picker.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.begin();
  Serial.begin(115200);

  if (!SD.begin()) {
    Serial.println("Failed to initalize the SD card.");
  }

  WIFI::connect();
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.update();
  Picker::loop();
}