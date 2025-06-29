#include <all.hpp>

class M_MQTT {
   private:
    static const char *_aliveTemplate;
    static const uint16_t _maxAttempts = maxAttempts;
    static const uint8_t _idSize = tinyBufferSize;
    static const uint8_t _topicSize = mediumBufferSize;
    static const uint8_t _replySize = bigBufferSize;
    static const uint8_t _messageSize = mediumBufferSize;
   public:
    bool ready = false;
    enum TopicName { T_HEALTH = 0, T_LOG, T_CMD, T_DATA, T_COUNT };

    bool setup(const char *host, const uint16_t port);
    bool check(void);
    bool loop(void);
    void send(TopicName topic, const char *msg);
    void sendf(TopicName topic, const char *format, ...);

   private:
    char _id[_idSize];
    char _topics[T_COUNT][_topicSize];
    char _reply[_replySize];

    bool makeTopic(char *topic, const char *prefix);
};

class LogPrint : public Print {
    private:
        static const uint8_t _bufferSize = bigBufferSize;

   public:
    void setPrint(Print &print);
    void setMQTT( M_MQTT &mqtt, M_MQTT::TopicName topic);
    size_t write(uint8_t);
    int availableForWrite();

   private:
    bool _usePrint = false;
    Print *_print;
    bool _useMQTT = false;
    M_MQTT *_mqtt;
    M_MQTT::TopicName _topic;
    uint8_t _bufferPos = 0;
    char _buffer[_bufferSize];
};
