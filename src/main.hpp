#pragma once

#include "all.hpp"

// task scheduler
// #define _TASK_SLEEP_ON_IDLE_RUN
#include <TaskScheduler.h>

#include <ArduinoOTA.h>

// pickup secrets from command line
#include "secrets.hpp"

// local libraries
#include "n_log.hpp"
#include "n_nmea.hpp"
#include "n_udp_data.hpp"
#include "n_wifi.hpp"
