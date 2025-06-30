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
void mqttCallback(char *topic, byte *payload, unsigned int length);

// initialize library instances
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// initialize Globals
M_MQTT mqtt;
LogPrint logPrint;

// constants
const char *M_MQTT::_aliveTemplate = mqttAliveTemplate;

bool M_MQTT::makeTopic(char *topic, const char *prefix) {
    size_t n = strlcpy(topic, prefix, _topicSize);
    if (n >= _topicSize) {
        Log.fatalln("MQTT prefix too long for topic: %s", prefix);
        return false;
    }
    size_t left = _topicSize - n;
    if (strlcpy(topic + n, _id, left) >= left) {
        Log.fatalln("MQTT prefix too long for topic: %s", prefix);
        return false;
    }
    return true;
}

bool M_MQTT::setup(const char *host, const uint16_t port) {
    bool ret = true;

    if (strlcpy(_id, WiFi.getHostname(), _idSize) >= _idSize) {
        Log.fatalln("MQTT ID too big");
        return false;
    }
    ret &= makeTopic(_topics[T_HEALTH], mqttHealthTopic);
    ret &= makeTopic(_topics[T_LOG], mqttLogTopic);
    ret &= makeTopic(_topics[T_CMD], mqttCmdTopic);
    ret &= makeTopic(_topics[T_DATA], mqttDataTopic);
    Log.verboseln("Setting mqq host %s and port %d", host, port);
    mqttClient.setServer(host, port);
    mqttClient.setCallback(mqttCallback);
    return ret;
}

bool M_MQTT::check(void) {
    uint8_t count = 0;

    while (!mqttClient.connected()) {
        if (mqttClient.connect(_id, _topics[T_HEALTH], 0, false,
                               "Disconnected")) {
            char msg[_messageSize];
            snprintf(msg, _messageSize, _aliveTemplate, WiFi.localIP().toString().c_str());
            send(T_HEALTH, msg);
            mqttClient.subscribe(_topics[T_CMD]);
            ready = true;
            break;
        } else {
            ready = false;
            if (++count > _maxAttempts) {
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

void M_MQTT::send(TopicName topic, const char *msg) {
    if (mqttClient.connected()) {
        mqttClient.publish(_topics[topic], msg);
    }
}

void LogPrint::startPrint(Print &print) {
    _print = &print;
    _usePrint = true;
}

bool LogPrint::startPrint(void) {
    if (_print) {
        _usePrint = true;
    } else {
        Log.error("Cannot start, print mechanism not set");
        return false;
    }
    return true;
}


void LogPrint::startMQTT(M_MQTT &mqtt, M_MQTT::TopicName topic) {
    _mqtt = &mqtt;
    _topic = topic;
    _useMQTT = true;
}


bool LogPrint::startMQTT(void) {
    if (_mqtt && _topic) {
        _useMQTT = true;
    } else {
        Log.error("Cannot start, mqtt mechanism not set");
        return false;
    }
    return true;
}

size_t LogPrint::write(uint8_t byte) {
    uint8_t wrap = '\0';

    switch (byte) {
        case '\0':
        case '\r':
            return 1;
        case '\n':
            byte = '\0';
            break;
        default:;
    }
    if ((_bufferPos + 1) == _bufferSize) {
        // if we will overflow send what we have
        // and continue from the beginning
        wrap = byte;
        byte = '\0';
    }
    _buffer[_bufferPos++] = byte;
    if (byte == '\0') {
        if (_usePrint) _print->println(_buffer);
        if (_useMQTT) _mqtt->send(_topic, _buffer);
        _bufferPos = 0;
    }
    if (wrap) {
        _buffer[_bufferPos++] = wrap;
    }
    return 1;
}

int LogPrint::availableForWrite() { return _bufferSize - _bufferPos; }
