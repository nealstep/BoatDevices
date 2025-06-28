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

// global variables
IPAddress localIP;
uint32_t currentMillis;
uint32_t previousMillis;
uint32_t seconds;
bool ledStatus = false;
bool reconnect = false;

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
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
    streamCmd.setStream(Serial);
    // settling time
    delay(delayLong);
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
            if (!mqtt.check()) {
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
        while (true) delay(delayLong);
    }
}

void blink(void) {
    Log.verboseln("blink");
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN, ledStatus);
}

bool cmdHandle(const char *cmd) {
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
                ret = false;
        }
    } else {
        Log.errorln("No command");
    }
    return ret;
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Log.warningln("mqttCallback Not Implemented");
    Log.verboseln("callback on topic %s size %d payload: %s", topic, length,
                  payload);
}

void loop() {
    // check for a reconnect
    if (reconnect == true) {
        mqtt.check();
        reconnect = false;
    }
    // check ota
    ArduinoOTA.handle();
    // mqtt check
    mqtt.loop();
    // check stream cmd
    if (streamCmd.loop()) {
        cmdHandle(streamCmd.get());
    }
    // count delayLongs (seconds)
    currentMillis = millis();
    if (currentMillis - previousMillis >= second) {
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
            Log.verboseln("MQTT Check Time");
            mqtt.check();
            mqttDone = true;
        }
    }
}
