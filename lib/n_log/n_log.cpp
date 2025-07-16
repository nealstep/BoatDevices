#include "n_log.hpp"

#include "n_wifi.hpp"

#ifdef USING_UDP_LOG
static const char *udp_log_ip = MY_UDP_LOG_IP;
static const uint16_t udp_log_port = MY_UDP_LOG_PORT;
#endif  // USING_UDP_LOG

void n_log(const char *format, ...) {
#if defined(USING_SERIAL) || defined(USING_UDP_LOG)
    char buffer[buffer_size];
    const char *msg;
    size_t len;
    va_list args;

    va_start(args, format);
    len = vsnprintf(buffer, buffer_size, format, args);
    va_end(args);
    if ((len >= 0) && (len < buffer_size))
        msg = buffer;
    else
        msg = err_too_long;
#ifdef USING_SERIAL
    if (use_serial) {
        Serial.println(msg);
    }
#endif  // USING_SERIAL
#ifdef USING_UDP_LOG
    if (use_udp) {
        if (udp_log_ip[0] == '0') {
            udp.beginPacket(broadcast, udp_log_port);
        } else {
            udp.beginPacket(udp_log_ip, udp_log_port);
        }
        udp.print(msg);
        udp.endPacket();
    }
#endif  // USING_UDP_LOG
#endif  // USING_SERIAL or USING_UDP_LOG
}
