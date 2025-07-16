#pragma once

#include "all.hpp"

#ifdef USING_NMEA

void send_nmea(const char *msg, bool chksum = false);

#endif // USING_NMEA