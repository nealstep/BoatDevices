#include <all.hpp>

class M_MQTT {
   private:
    static const char *_aliveTemplate;
    static const uint16_t _maxConnects = mqttMaxConnects;

   public:
    bool ready = false;

    bool setup(const char *host, const uint16_t port);
    bool check(void);
    bool loop(void);

   private:
    char _id[mqttIDSize];
    enum topicNames { HEALTH = 0, LOG, CMD, DATA, COUNT };
    char _topics[COUNT][mqttTopicSize];

    bool makeTopic(char *topic, const char *prefix);
};
