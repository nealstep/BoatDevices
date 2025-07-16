#pragma once

#include <Arduino.h>

/*
    These are passed in secret.ini via compiler DEFINES
*/

#ifndef MY_BAUD
#error Need to pass MY_BAUD
#endif // MY_BAUD
#ifndef MY_SSID
#error Need to pass MY_SSID
#endif // MY_SSID
#ifndef MY_PASSWD
#error Need to pass MY_PASSWD
#endif // MY_PASSWD
#ifndef MY_OTA_PASSWD
#error Need to pass MY_OTA_PASSWD
#endif // MY_OTA_PASSWD
#ifndef MY_UDP_LOG_IP
#error Need to pass MY_UDP_LOG_IP
#endif // MY_UDP_LOG_IP
#ifndef MY_UDP_LOG_PORT
#error Need to pass MY_UDP_LOG_PORT
#endif // MY_UDP_LOG_PORT
#ifndef MY_UDP_DATA_IP
#error Need to pass MY_UDP_DATA_IP
#endif // MY_UDP_DATA_IP
#ifndef MY_UDP_DATA_PORT
#error Need to pass MY_UDP_DATA_PORT
#endif // MY_UDP_DATA_PORT
#ifndef MY_NMEA_IP
#error Need to pass MY_NMEA_IP
#endif // MY_NMEA_IP
#ifndef MY_NMEA_PORT
#error Need to pass MY_NMEA_PORT
#endif // MY_NMEA_PORT
