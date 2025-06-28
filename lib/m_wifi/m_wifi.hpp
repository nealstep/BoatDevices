#pragma once

#include "all.hpp"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Not supported"
#endif  // ESP8266 - ESP32

class M_WiFi {
   private:
    const uint16_t _connectDelay = 500;
    const uint8_t _maxAttempts = wifiMaxAttempts;

   public:
    bool init(const char *wifi_ssid, const char *wifi_passwd);
    bool connected(void);

   private:
    bool copyStrings(const char *wifiSSID, const char *wifiPasswd);
};
