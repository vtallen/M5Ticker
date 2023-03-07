#include <Arduino.h>
#include <M5Core2.h>
#include <WiFi.h>

#include <helpers/wifi_helper.h>

namespace WIFI {

String ssid;
String password;

bool loadConfigFile() {
  File file = SD.open("/wifi.txt");
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      int delimiterIndex = line.indexOf(':');
      if (delimiterIndex > 0) {
        ssid = line.substring(0, delimiterIndex);
        password = line.substring(delimiterIndex + 1);

        ssid.trim();
        password.trim();

        return true;
      }
    }
    file.close();
  } else {
    Serial.println("Error opening wifi.txt");
  }

  return false;
}

bool connect() {
  if (loadConfigFile()) {
    Serial.println("WiFi SSID and password loaded from wifi.txt");
  } else {
    return false;
  }

  assert(ssid && "ssid variable was not set");
  assert(password && "password variable was not set");

  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    if ((startTime - millis()) >= 10000) {
      break;
    }
  }

  if (WL_CONNECTED) {
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("WiFi connection failed!");
    return false;
  }
}
} // namespace WIFI