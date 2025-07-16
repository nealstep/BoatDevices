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
#ifdef USING_M_MQTT
#ifndef MY_MQTT_HOST
#error Need to pass MY_MQTT_HOST
#endif // MY_MQTT_HOST
#ifndef MY_MQTT_PORT
#error Need to pass MY_MQTT_PORT
#endif // MY_MQTT_PORT
#endif // USING_M_MQTT
#ifdef USING_M_UDP
#ifndef MY_UDP_LOG_HOST
#error Need to pass MY_UDP_LOG_HOST
#endif // MY_UDP_LOG_HOST
#ifndef MY_UDP_LOG_PORT
#error Need to pass MY_UDP_LOG_PORT
#endif // MY_UDP_LOG_PORT
#ifndef MY_UDP_DATA_HOST
#error Need to pass MY_UDP_DATA_HOST
#endif // MY_UDP_DATA_HOST
#ifndef MY_UDP_DATA_PORT
#error Need to pass MY_UDP_DATA_PORT
#endif // MY_UDP_DATA_PORT
#endif // USING_M_UDP


// convert compile time defines to c++ globals
const uint32_t baud = MY_BAUD;
const char *wifiSSID = MY_WIFI_SSID;
const char *wifiPasswd = MY_WIFI_PASSWD;
const char *otaPasswd = MY_OTA_PASSWD;
#ifdef USING_M_MQTT
const char *mqttHost = MY_MQTT_HOST;
const uint16_t mqttPort = MY_MQTT_PORT;
#endif // USING_M_MQTT
#ifdef USING_M_UDP
const char *udpLogHost = MY_UDP_LOG_HOST;
const uint16_t udpLogPort = MY_UDP_LOG_PORT;
const char *udpDataHost = MY_UDP_DATA_HOST;
const uint16_t udpDataPort = MY_UDP_DATA_PORT;
#endif // USING_M_UDP
