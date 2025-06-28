#pragma once

/*
    These are passed in secret.ini via compiler DEFINES
*/

#ifndef MY_BAUD
#error Need to pass MY_BAUD
#endif // MY_BAUD
#ifndef MY_WIFI_SSID
#error Need to pass MY_WIFI_SSID
#endif // MY_SSID
#ifndef MY_WIFI_PASSWD
#error Need to pass MY_WIFI_PASSWD
#endif // MY_PASSWD
#ifndef MY_OTA_PASSWD
#error Need to pass MY_OTA_PASSWD
#endif // MY_OTA_PASSWD
#ifndef MY_MQTT_HOST
#error Need to pass MY_MQTT_HOST
#endif // MY_OTA_PASSWD
#ifndef MY_MQTT_PORT
#error Need to pass MY_MQTT_PORT
#endif // MY_OTA_PASSWD

// convert compile time defines to c++ globals
const uint32_t baud = MY_BAUD;
const char *wifiSSID = MY_WIFI_SSID;
const char *wifiPasswd = MY_WIFI_PASSWD;
const char *otaPasswd = MY_OTA_PASSWD;
const char *mqttHost = MY_MQTT_HOST;
const uint16_t mqttPort = MY_MQTT_PORT;
