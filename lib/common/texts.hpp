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

// messages
constexpr const char *msg_creating_fields = "Creating fields";
constexpr const char *msg_setting_defaults = "Setting defaults";
constexpr const char *msg_starting = "Starting";
constexpr const char *msg_wifi_connected_1 = "WiFi connected IP: %p";
constexpr const char *msg_invalid_log_level_1 = "Invalid log level: %s";
