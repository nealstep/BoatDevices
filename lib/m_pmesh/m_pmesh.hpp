#pragma once

class PMesh {
    ;

};

class LogPrint : public Print {
    private:
        static const uint8_t _bufferSize = bigBufferSize;

   public:
    void startPrint(Print &print);
    bool startPrint(void);
    void stopPrint(void) {_usePrint = false;}
    void startMQTT(M_MQTT &mqtt, M_MQTT::TopicName topic);
    bool startMQTT(void);
    void stopMQTT(void) {_useMQTT = false;}
    size_t write(uint8_t);
    int availableForWrite();

   private:
    bool _usePrint = false;
    Print *_print = nullptr;
    bool _useMQTT = false;
    M_MQTT *_mqtt = nullptr;
    M_MQTT::TopicName _topic;
    uint8_t _bufferPos = 0;
    char _buffer[_bufferSize];
};

// export globals
extern M_MQTT mqtt;
extern LogPrint logPrint;