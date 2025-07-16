#pragma once

#include <Arduino.h>

// delays
extern const uint16_t delay_medium;
extern const uint16_t delay_long;

// stadard sizes
extern const uint8_t buffer_size;
constexpr uint8_t key_length = 16;
constexpr uint8_t max_keys = 12;

// rates
extern const uint16_t two_hz;
extern const uint16_t one_hz;
extern const uint16_t ota_rate;

// errors
extern const char *err_too_long;
extern const char *err_too_long_1;
extern const char *err_data_too_long;
extern const char *err_disconnected;
extern const char *err_input_before_cleared;
extern const char *err_buffer_overrun;

// messages (no arguments)
extern const char *msg_blink;
extern const char *msg_connected;
extern const char *msg_connecting;
extern const char *msg_reconnecting;
extern const char *msg_started;
extern const char *msg_setup_done;

// messages with arguments
extern const char *msg_ip_4;
extern const char *msg_diconnect_reason_1;
extern const char *msg_level1_1;

// global variables
extern bool use_serial;
extern bool use_udp;
extern bool use_nmea;

// wifi globals variables
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "ESP8266 or ESP32 required"
#endif  // ESP8266 or ESP32
#include <WiFiUdp.h>

extern WiFiUDP udp;
extern IPAddress broadcast;