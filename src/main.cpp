#include <ArduinoOTA.h>

#include "all.hpp"
#include "m_config.hpp"
#include "m_mqtt.hpp"
#include "m_stream_cmd.hpp"
#include "m_wifi.hpp"
#include "secrets.hpp"

// initialize library instances
StreamCmd streamCmd;
M_Config config;
M_MQTT mqtt;
M_WiFi wifi;
LogPrint logPrint;

// constants
const uint8_t mqttCmdSize = mediumBufferSize;

// global variables
IPAddress localIP;
uint32_t currentMillis;
uint32_t previousMillis;
uint32_t seconds;
bool ledStatus = false;
bool reconnect = false;
uint32_t mqttLastCheck = 0;
bool mqttCmdRdy = false;
char mqttCmd[mqttCmdSize];

// task globals
bool blinkDone = false;
bool mqttDone = false;

// config data
void configSetup(void) {
    Log.traceln("configSetup");
    Log.verboseln("Creating fields");
    config.add("wifi_ssid", config.CHARS, 32);
    config.add("wifi_passwd", config.CHARS, 32);
    config.add("mqtt_host", config.CHARS, 32);
    config.add("mqtt_port", config.U16);
    // set defaults, if desired
    Log.verboseln("Setting defaults");
    config.set("wifi_ssid", wifiSSID);
    config.set("wifi_passwd", wifiPasswd);
    config.set("mqtt_host", mqttHost);
    config.set("mqtt_port", mqttPort);
    // check we have created everythning
    uint8_t id = 0;
    const char *name;
    while (true) {
        name = config.getName(id++);
        if (!name) break;
        Log.noticeln("config name %d: %s", id, name);
    }
}

// setup
void setup() {
    Serial.begin(baud);
    logPrint.setPrint(Serial);
    Log.begin(LOG_LEVEL_VERBOSE, &logPrint, true);
    streamCmd.setStream(Serial);
    // settling time
    delay(longDelay);
    Log.traceln("Starting");
    // blink setup
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, ledStatus);
    // setup config
    configSetup();
    // setup wifi
    const char *ssid;
    const char *passwd;
    config.get("wifi_ssid", ssid);
    config.get("wifi_passwd", passwd);
    if (wifi.init(ssid, passwd)) {
        localIP = WiFi.localIP();
        Log.noticeln("WiFi connected IP: %p", localIP);
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
                logPrint.setMQTT(mqtt, mqtt.T_LOG);
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

bool cmdHandle(const char *cmd, bool useMQTT = false) {
    bool ret = true;
    uint8_t len = strlen(cmd);

    Log.traceln("cmdHandle cmd: %s", cmd);
    if (len > 0) {
        switch (cmd[0]) {
            // case 'C':
            //     ret = cmdConfig(&(cmd[1]), len - 1);
            //     break;
            default:
                Log.warningln("Unknown cmd: %s", cmd);
                if (useMQTT) {
                    mqtt.sendf(mqtt.T_CMD, "* Unknown cmd: %s", cmd);
                }
                ret = false;
        }
    } else {
        Log.errorln("No command");
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
