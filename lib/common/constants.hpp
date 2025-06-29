#pragma once

#include <Arduino.h>

// time constants
const uint32_t shortDelay = 50;
const uint32_t mediumDelay = 500;
const uint32_t longDelay = 5000;
const uint32_t millis_in_second = 1000;

// scheduling constants
const uint16_t blinkTime = 5;
const uint16_t blinkOffset = 0;
const uint16_t mqttCheckTime = 30;
const uint16_t mqttCheckTimeOffset = 5;
const uint16_t mqttObsessTime = 2;

// size constants
const uint8_t tinyBufferSize = 16;
const uint8_t smallBufferSize = 32;
const uint8_t mediumBufferSize = 64;
const uint8_t bigBufferSize = 192;

// count constants
const uint8_t configItems = 10;
const uint8_t maxAttempts = 10;
