#include "n_udp_data.hpp"

#ifdef USING_UDP_DATA

#include "n_wifi.hpp"

static const char *udp_data_ip = MY_UDP_DATA_IP;
static const uint16_t udp_data_port = MY_UDP_DATA_PORT;

void send_data(const char *fmt, ...) {
    char buffer[buffer_size];
    const char *msg;
    size_t len;
    va_list args;

    if (use_udp) {
        va_start(args, fmt);
        len = vsnprintf(buffer, buffer_size, fmt, args);
        va_end(args);
        if ((len >= 0) && (len < buffer_size))
            msg = buffer;
        else
            msg = err_data_too_long;
        if (udp_data_ip[0] == '0') {
            udp.beginPacket(broadcast, udp_data_port);
        } else {
            udp.beginPacket(udp_data_ip, udp_data_port);
        }
        udp.print(msg);
        udp.endPacket();
    }
}

#endif  // USING_UDP_DATA