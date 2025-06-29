#pragma once

#include "all.hpp"

#include <ArduinoJson.h>

class M_Config {
   public:
    enum DataTypeCodes { ANY, CHARS, U16, I16, BOOL };

   private:
    static const uint8_t _badItem = 255;
    static const uint8_t _filenameSize = smallBufferSize;
    static const uint8_t _nameSize = tinyBufferSize;
    static const uint8_t _items = configItems;
    static const char _defaultFilename[_filenameSize];
    union DataTypes {
        char *chars;
        uint16_t u16;
        int16_t i16;
        bool b;
    };
    struct DataPair {
        char name[_nameSize];
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
    bool get(uint8_t id, bool &datum);

    bool get(const char *name, const char *&datum);
    bool get(const char *name, uint16_t &datum);
    bool get(const char *name, int16_t &datum);
    bool get(const char *name, bool &datum);

    bool set(const char *name, const char *datum);
    bool set(const char *name, const uint16_t &datum);
    bool set(const char *name, const int16_t &datum);
    bool set(const char *name, const bool &datum);

   private:
    char _filename[_filenameSize];
    DataPair _config[_items];
    uint8_t _configCount = 0;
    DeserializationError _dError;
};

// export globals
extern M_Config config;

// command routines
bool cmdConfig(const char *cmd, uint8_t len);
