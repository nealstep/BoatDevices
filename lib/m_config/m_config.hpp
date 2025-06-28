#pragma once

#include <ArduinoJson.h>

#include "all.hpp"

class M_Config {
   public:
    enum DataTypeCodes { ANY, CHARS, U16, I16 };

   private:
    static const uint8_t _badItem = 255;
    static const char _defaultFilename[filenameSize];
    union DataTypes {
        char *chars;
        uint16_t u16;
        int16_t i16;
    };
    struct DataPair {
        char name[configNameSize];
        DataTypeCodes type;
        uint8_t size;
        DataTypes datum;
    };

   public:
    M_Config(const char *filename = _defaultFilename);
    bool exists(void);
    bool read(void);
    bool write(void);

    bool add(const char *name, DataTypeCodes type, uint8_t size = 0);

    uint8_t find(const char *name, DataTypeCodes type = ANY);
    const char *getName(uint8_t item);
    DataTypeCodes getType(uint8_t item);
    DataTypeCodes getType(const char *name);

    bool get(uint8_t id, const char *&datum);
    bool get(uint8_t id, uint16_t &datum);
    bool get(uint8_t id, int16_t &datum);

    bool get(const char *name, const char *&datum);
    bool get(const char *name, uint16_t &datum);
    bool get(const char *name, int16_t &datum);

    bool set(const char *name, const char *datum);
    bool set(const char *name, const uint16_t &datum);
    bool set(const char *name, const int16_t &datum);

   private:
    char _filename[filenameSize];
    DataPair _config[configItems];
    uint8_t _configCount = 0;
    DeserializationError _dError;
};
