#pragma once

#include <Arduino.h>
#include <ArduinoLog.h>

#include "texts.hpp"

// time constants
const uint32_t delayShort = 50;
const uint32_t delayMedium = 500;
const uint32_t delayLong = 5000;
const uint32_t second = 1000;

// scheduling constants
const uint16_t blinkTime = 5;
const uint16_t blinkOffset = 0;
const uint16_t mqttCheckTime = 30;
const uint16_t mqttCheckTimeOffset = 5;

// size constants
const uint8_t mqttIDSize = 16;
const uint8_t mqttTopicSize = 64;
const uint8_t mqttMessageSize = 64;
const uint8_t configItems = 10;
const uint8_t configNameSize = 16;
const uint8_t filenameSize = 32;
const uint8_t wifiStringSize = 32;

// count constants
const uint8_t mqttMaxConnects = 5;
const uint8_t wifiMaxAttempts = 10;