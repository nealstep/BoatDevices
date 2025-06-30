#include <all.hpp>

class M_MQTT {
   private:
    static const char *_aliveTemplate;
    static const uint16_t _maxAttempts = maxAttempts;
    static const uint8_t _idSize = tinyBufferSize;
    static const uint8_t _topicSize = mediumBufferSize;
    static const uint8_t _messageSize = mediumBufferSize;
   public:
    bool ready = false;
    enum TopicName { T_HEALTH = 0, T_LOG, T_CMD, T_DATA, T_COUNT };

    bool setup(const char *host, const uint16_t port);
    bool check(void);
    bool loop(void);
    void send(TopicName topic, const char *msg);

   private:
    char _id[_idSize];
    char _topics[T_COUNT][_topicSize];

    bool makeTopic(char *topic, const char *prefix);
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
