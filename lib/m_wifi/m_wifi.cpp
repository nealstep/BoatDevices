#include "m_wifi.hpp"

extern bool reconnect;

static char _ssid[M_WiFi::stringSize];
static char _passwd[M_WiFi::stringSize];

bool M_WiFi::copyStrings(const char *wifiSSID, const char *wifiPasswd) {
    if (strlcpy(_ssid, wifiSSID, stringSize) >= stringSize) {
        Log.fatal("WiFi init unable to copy SSID: %s", wifiSSID);
        return false;
    }
    if (strlcpy(_passwd, wifiPasswd, stringSize) >= stringSize) {
        Log.fatal("WiFi init unable to copy passwd");
        return false;
    }
    return true;
}

#ifdef ESP8266

WiFiEventHandler wifiDisconnectHandler;

void WiFiStationDisconnected(const WiFiEventStationModeDisconnected &event) {
    WiFi.disconnect();
    WiFi.begin(_ssid, _passwd);
    reconnect = true;
}

#endif  // ESP8266

#ifdef ESP32

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    WiFi.disconnect(true);
    WiFi.begin(_ssid, _passwd);
    reconnect = true;
}

#endif  // ESP32

bool M_WiFi::init(const char *wifiSSID, const char *wifiPasswd) {
    uint8_t attempts = 0;

    if (!copyStrings(wifiSSID, wifiPasswd)) return false;
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
#ifdef ESP8266
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(WiFiStationDisconnected);
#endif  // ESP8266
#ifdef ESP32
    WiFi.onEvent(WiFiStationDisconnected,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
#endif  // ESP32
    WiFi.begin(_ssid, _passwd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(_connectDelay);
        if (attempts++ > _maxAttempts) {
            return false;
        }
    }
    return true;
}

bool M_WiFi::connected(void) { return WiFi.status() == WL_CONNECTED; }
