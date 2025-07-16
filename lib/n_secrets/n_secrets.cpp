#include "secrets.hpp"

#include "n_config.hpp"

static const uint32_t baud = MY_BAUD;

static const char* ssid = MY_SSID;
static const char* passwd = MY_PASSWD;

static const char *ota_passwd = MY_OTA_PASSWD;

static const char *udp_log_ip = MY_UDP_LOG_IP;
static const uint16_t udp_log_port = MY_UDP_LOG_PORT;

static const char *udp_data_ip = MY_UDP_DATA_IP;
static const uint16_t udp_data_port = MY_UDP_DATA_PORT;

static const char *nmea_ip = MY_NMEA_IP;
static const uint16_t nmea_port = MY_NMEA_PORT;

void setup_secrets (void) {
    config.read();
    if (config.key_count()) {
        return;
    }

    // write secrets to config
}