#include "n_nmea.hpp"

#ifdef USING_NMEA

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "ESP8266 or ESP32 required"
#endif  // ESP8266 or ESP32

extern bool use_udp;
extern WiFiUDP udp;
extern IPAddress broadcast;

#include "n_log.hpp"

static const char *nmea_ip = MY_NMEA_IP;
static const uint16_t nmea_port = MY_NMEA_PORT;

// nmea checksum
uint8_t checksum(const char *data) {
    uint8_t crc = 0;

    for (uint8_t i = 1; i < strlen(data); i++) {
        crc ^= data[i];
    }
    return crc;
}

void send_nmea(const char *msg, bool chksum) {
    uint8_t crc;
    char chksum_str[5];

    if (use_nmea) {
        if (chksum) {
            if (!snprintf(chksum_str, 5, "*%X", checksum(msg))) {
                n_log("chksum overflowed");
                chksum_str[0] = '\0';
            }
        }
        udp.beginPacket(nmea_ip, nmea_port);
        udp.print(msg);
        if (chksum) udp.print(chksum_str);
        udp.endPacket();
    }
}


#endif // USING_NMEA