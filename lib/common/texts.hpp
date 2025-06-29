#pragma once

// mqtt topics roots
constexpr const char *mqttHealthTopic = "/devices/health/";
constexpr const char *mqttLogTopic = "/devices/log/";
constexpr const char *mqttDataTopic = "/devices/data/";
constexpr const char *mqttCmdTopic = "/devices/cmd/";

// mqtt alive template
constexpr const char *mqttAliveTemplate = "alive %s";

// mqtt messages
constexpr const char *mqttWait = "Wait still processing last command";
