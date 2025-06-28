#include "m_mqtt.hpp"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Not supported"
#endif  // ESP8266 - ESP32
#include <PubSubClient.h>

// extrernals
extern IPAddress localIP;
void mqttCallback(char *topic, byte *payload, unsigned int length);

// initialize library instances
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// constants
const char *M_MQTT::_aliveTemplate = mqttAliveTemplate;


bool M_MQTT::makeTopic(char *topic, const char *prefix) {
    size_t n = strlcpy(topic, prefix, mqttTopicSize);
    if (n >= mqttTopicSize) {
        Log.fatalln("MQTT prefix too long for topic: %s", prefix);
        return false;
    }
    size_t left = mqttTopicSize - n;
    if (strlcpy(topic + n, _id, left) >= left) {
        Log.fatalln("MQTT prefix too long for topic: %s", prefix);
        return false;
    }
    return true;
}

bool M_MQTT::setup(const char *host, const uint16_t port) {
    bool ret = true;

    if (strlcpy(_id, WiFi.getHostname(), mqttIDSize) >= mqttIDSize) {
        Log.fatalln("MQTT ID too big");
        return false;
    }
    ret &= makeTopic(_topics[HEALTH], mqttHealthTopic);
    ret &= makeTopic(_topics[LOG], mqttLogTopic);
    ret &= makeTopic(_topics[CMD], mqttCmdTopic);
    ret &= makeTopic(_topics[DATA], mqttDataTopic);
    Log.verboseln("Setting mqq host %s and port %d", host, port);
    mqttClient.setServer(host, port);
    mqttClient.setCallback(mqttCallback);
    return ret;
}

bool M_MQTT::check(void) {
    uint8_t count = 0;

    while (!mqttClient.connected()) {
        if (mqttClient.connect(_id, _topics[HEALTH], 0, false,
                               "Disconnected")) {
            char msg[mqttMessageSize];
            snprintf(msg, mqttMessageSize, _aliveTemplate,
                     localIP.toString().c_str());
            mqttClient.publish(_topics[HEALTH], msg);
            mqttClient.subscribe(_topics[CMD]);
            // Need to setup log engine to switch to mqtt or use both
            ready = true;
            Log.noticeln("MQTT connnected");
            break;
        } else {
            ready = false;
            if (++count > mqttMaxConnects) {
                Log.errorln("MQTT connection failed");
                return false;
            }
        }
    }
    return true;
}

bool M_MQTT::loop(void) {
    if (!mqttClient.loop()) {
        ready = false;
        if (check()) {
            return mqttClient.loop();
        } else {
            return false;
        }
    }
    return true;
}
