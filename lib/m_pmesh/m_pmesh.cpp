#include "m_pmesh.hpp"



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