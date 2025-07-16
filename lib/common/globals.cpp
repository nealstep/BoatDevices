#include "globals.hpp"

// standard delays
const uint16_t delay_medium = 250;
const uint16_t delay_long = 1000;

// stadard sizes
const uint8_t buffer_size = 128;

// rates
const uint16_t two_hz = 500;
const uint16_t one_hz = 1000;
const uint16_t ota_rate = 10;

// errors
const char *err_too_long = "! Message too long: m_log";
const char *err_too_long_1 = "! Message too long: %s";
const char *err_data_too_long = "! Data too long";
const char *err_disconnected = "! Disconnected";
const char *err_input_before_cleared = "! Input before cleared";
const char *err_buffer_overrun = "! Buffer overrun, restartarting";

// messages (no arguments)
const char *msg_blink = "* Blink";
const char *msg_connected = "* Connected to WiFi";
const char *msg_connecting = "* Connecting to WiFi";
const char *msg_reconnecting = "* Reconnecting to WiFi";
const char *msg_started = "* Started";
const char *msg_setup_done = "* Setup Done";

// messages with arguments
const char *msg_ip_4 = "* Got IP: %hhu.%hhu.%hhu.%hhu";
const char *msg_diconnect_reason_1 = "* WiFi lost connection. Reason: %s";
const char *msg_level1_1 = "* Level 1 reading: %.0f";

// global variables
bool use_serial = false;
bool use_udp = false;
bool use_nmea = false;

// wifi global variables
WiFiUDP udp;
IPAddress broadcast;