#include "n_wifi.hpp"

#include "n_log.hpp"
#include "n_nmea.hpp"

static const char* ssid = MY_SSID;
static const char* passwd = MY_PASSWD;

#ifdef ESP8266
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
    IPAddress ip = WiFi.localIP();
    broadcast = WiFi.broadcastIP();

    use_udp = true;
    use_nmea = true;
    n_log(msg_ip_4, ip[0], ip[1], ip[2], ip[3]);
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
    use_udp = false;
    use_nmea = false;
    n_log(err_disconnected);
    n_log(msg_reconnecting);
    WiFi.begin(ssid, passwd);
}

void wifi_setup(void) {
    IPAddress ip;

    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
    WiFi.mode(WIFI_STA);
    n_log(msg_connecting);
    WiFi.begin(ssid, passwd);
    while (true) {
        if (use_udp) break;
        delay(delay_medium);
    }
}
#endif  // ESP8266

#ifdef ESP32
void WiFi_Connected(WiFiEvent_t event, WiFiEventInfo_t info) {
    n_log(msg_connected);
}

void WiFi_IP(WiFiEvent_t event, WiFiEventInfo_t info) {
    IPAddress ip = WiFi.localIP();
    broadcast = WiFi.broadcastIP();

    use_udp = true;
    use_nmea = true;
    n_log(msg_ip_4, ip[0], ip[1], ip[2], ip[3]);
}

void WiFi_Disconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    use_udp = false;
    use_nmea = false;
    n_log(err_disconnected);
    n_log(msg_diconnect_reason_1, info.wifi_sta_disconnected.reason);
    n_log(msg_reconnecting);
    WiFi.begin(ssid, passwd);
}

void wifi_setup(void) {
    WiFi.onEvent(WiFi_Connected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFi_IP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFi_Disconnected,
                 WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    n_log(msg_connecting);
    WiFi.begin(ssid, passwd);
    while (true) {
        if (use_udp) break;
        delay(delay_medium);
    }
}
#endif  // ESP32
