#include "main.hpp"

static const char *c_version = "version .01";
static const uint32_t baud = MY_BAUD;
static const char *ota_passwd = MY_OTA_PASSWD;

// object globals
static Scheduler scheduler;

// globals
static bool led_state = false;

// tasks
void blink(void);
void aOTA(void);
Task tBlink(two_hz, TASK_FOREVER, &blink);
Task tOTA(ota_rate, TASK_FOREVER, &aOTA);

// blink the led
void blink(void) {
    n_log("blink");
    led_state = not(led_state);
    digitalWrite(LED_BUILTIN, led_state);
}

// handle ota
void aOTA(void) { ArduinoOTA.handle(); }

// die while doing OTA if able
void die(void) {
    // if we have network check for OTA
    if (use_udp) {
        ArduinoOTA.handle();
    } else {
        delay(delay_medium);
    }
}

void setup() {
#ifdef USING_SERIAL
    // serial setup
    Serial.begin(baud);
    use_serial = true;
#endif  // USING_SERIAL
    n_log(msg_started);
    // init wwifi
    wifi_setup();
    // get ready for OTA
    ArduinoOTA.setPassword(ota_passwd);
    ArduinoOTA.begin();
    // send version
    n_log(c_version);
    // get ready to blink
    pinMode(LED_BUILTIN, OUTPUT);
    blink();
    // setup tasks
    scheduler.init();
    scheduler.addTask(tBlink);
    scheduler.addTask(tOTA);
    // enable tasks
    tBlink.enable();
    tOTA.enable();
    n_log(msg_setup_done);
}

void loop() { scheduler.execute(); }
