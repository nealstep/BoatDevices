#include <ArduinoOTA.h>
#include <TaskScheduler.h>

#include "all.hpp"
#include "m_config.hpp"
#include "m_stream_cmd.hpp"
#include "m_pmesh.hpp"
#include "secrets.hpp"

// flags
bool useSerial;
bool useMesh;

// led status
bool ledStatus = false;

// config data
void configSetup(void) {
    Log.traceln("configSetup");
    Log.verboseln(msg_creating_fields);
    config.add("wifi_ssid", config.CHARS, M_WiFi::stringSize);
    config.add("wifi_passwd", config.CHARS, M_WiFi::stringSize);
    config.add("use_serial", config.BOOL);
    config.add("use_mesh", config.BOOL);
    config.add("log_level", config.CHARS, tinyBufferSize);

    // set defaults
    Log.verboseln(msg_setting_defaults);
    config.set("wifi_ssid", wifiSSID);
    config.set("wifi_passwd", wifiPasswd);
    config.set("use_serial", true);
    config.set("use_mesh", true);
    config.set("log_level", default_log_level);

    // read config
    if (config.exists()) {
        Log.noticeln("reading_config");
        config.read();
    }
}

void setLogLevel(const char *logLevel) {
    if (strncmp(logLevel, "verbose", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_VERBOSE);
    } else if (strncmp(logLevel, "trace", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_TRACE);
    } else if (strncmp(logLevel, "notice", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_NOTICE);
    } else if (strncmp(logLevel, "warning", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_WARNING);
    } else if (strncmp(logLevel, "error", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_ERROR);
    } else if (strncmp(logLevel, "fatal", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_FATAL);
    } else if (strncmp(logLevel, "silent", tinyBufferSize) == 0) {
        Log.setLevel(LOG_LEVEL_SILENT);
    } else {
        Log.error(msg_invalid_log_level_1, logLevel);
    }
}

// setup
void setup() {
    const char *logLevel;

    // set up Log with null logger
    Log.begin(LOG_LEVEL_VERBOSE, &logPrint, true);
    // setup config
    configSetup();
    config.get("use_serial", useSerial);
    config.get("use_mqtt", useMQTT);
    config.get("log_level", logLevel);
    if (useSerial) {
        Serial.begin(baud);
        logPrint.startPrint(Serial);
        streamCmd.setStream(Serial);
    }
    // settling time
    delay(longDelay);
    Log.traceln(msg_starting);
    // blink setup
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, ledStatus);
    // setup wifi
    const char *ssid;
    const char *passwd;
    config.get("wifi_ssid", ssid);
    config.get("wifi_passwd", passwd);
    if (wifi.init(ssid, passwd)) {
        Log.noticeln(msg_wifi_connected_1, WiFi.localIP());
        // setup OTA
        ArduinoOTA.setPassword(otaPasswd);
        ArduinoOTA.begin();
        // setup MQTT
        const char *host;
        uint16_t port;
        config.get("mqtt_host", host);
        config.get("mqtt_port", port);
        if (mqtt.setup(host, port)) {
            if (mqtt.check()) {
                if (useMQTT) {
                    logPrint.startMQTT(mqtt, mqtt.T_LOG);
                }
                Log.noticeln("MQTT connnected");
            } else {
                Log.fatalln("MQTT check failed");
            }
        } else {
            Log.fatalln("MQTT setup failed");
        }
    } else {
        Log.fatalln("WiFi failed to connect");
    }
    previousMillis = millis();
    if (wifi.connected() && mqtt.ready) {
        Log.traceln("Setup Done");
    } else {
        Log.error("issue with wifi %T and/or mqtt %T, stopping",
                  wifi.connected(), mqtt.ready);
        while (true) delay(longDelay);
    }
}

void blink(void) {
    // Log.verboseln("blink");
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
}

void cmdReply(bool replyMQTT, const char *format, ...) {
    char reply[bigBufferSize];
    va_list args;

    va_start(args, format);
    reply[0] = '*';
    reply[1] = ' ';
    vsnprintf(&(reply[2]), bigBufferSize - 2, format, args);
    va_end(args);
    if (replyMQTT) {
        mqtt.send(mqtt.T_CMD, reply);
    } else {
        Serial.println(reply);
    }
}

bool cmdHandle(const char *cmd, bool replyMQTT = false) {
    bool ret = true;
    uint8_t len = strlen(cmd);

    Log.traceln("cmdHandle cmd: %s", cmd);
    cmdReply(replyMQTT, "Running: %s", cmd);
    if (len > 0) {
        switch (cmd[0]) {
            case 'C':
                ret = cmdConfig(&(cmd[1]), len - 1, replyMQTT);
                break;
            default:
                Log.warningln("Unknown cmd: %s", cmd);
                cmdReply(replyMQTT, "Unknown cmd: %s", cmd);
                ret = false;
        }
    } else {
        Log.errorln("No command");
        cmdReply(replyMQTT, "No command");
        ret = false;
    }
    return ret;
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    if ((length > 0) && (payload[0] == '*')) {
        // ignore our traffic
        return;
    }
    if (mqttCmdRdy) {
        Log.warningln("Command arrived while still processing last command");
        mqtt.send(mqtt.T_CMD, mqttWait);
        return;
    }
    if (length < mqttCmdSize) {
        memcpy(mqttCmd, payload, length);
        mqttCmd[length] = '\0';
        Log.verboseln("callback on topic %s size %d data: %s", topic, length,
                      mqttCmd);
        mqttCmdRdy = true;
    } else {
        Log.errorln("mqttCallback topix %s payload too large: %d", topic,
                    length);
    }
}

void loop() {
    // check for a reconnect
    if ((reconnect == true) && (wifi.connected()) &&
        ((seconds - mqttLastCheck) > mqttObsessTime)) {
        Log.warning("Reconnect");
        mqtt.check();
        reconnect = false;
        mqttLastCheck = seconds;
    }
    // check ota
    ArduinoOTA.handle();
    // mqtt check
    mqtt.loop();
    // check stream cmd
    if (streamCmd.loop()) {
        cmdHandle(streamCmd.get());
    }
    // check mqtt command
    if (mqttCmdRdy) {
        cmdHandle(mqttCmd, true);
        mqttCmdRdy = false;
    }
    // count delayLongs (seconds)
    currentMillis = millis();
    if (currentMillis - previousMillis >= millis_in_second) {
        seconds++;
        previousMillis = currentMillis;
        // reset dones
        mqttDone = false;
        blinkDone = false;
    }
    if ((seconds % blinkTime) == blinkOffset) {
        if (!blinkDone) {
            blink();
            blinkDone = true;
        }
    }
    if ((seconds % mqttCheckTime) == mqttCheckTimeOffset) {
        if (!mqttDone) {
            // Log.verboseln("MQTT Check Time");
            mqtt.check();
            mqttDone = true;
        }
    }
}
