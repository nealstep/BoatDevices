#pragma once

#include "all.hpp"

bool config_set(void);

class MultiVar {
   private:
    char type;
    union {
        double d;
        uint32_t u32;
        int32_t i32;
        const char *c;
    } Var;

   public:
    MultiVar(double v);
    MultiVar(uint32_t v);
    MultiVar(int32_t v);
    MultiVar(const char *v);
    MultiVar operator=(double v);
    MultiVar operator=(uint32_t v);
    MultiVar operator=(int32_t v);
    bool get(double &d);
    bool get(uint32_t &u32);
    bool get(int32_t &i32);
    bool get(const char *(&c));
};

class Config {
   private:
    char key[max_keys][key_length];
    uint8_t keys = 0;
    MultiVar value[max_keys];
    uint8_t bad_value = 255;

    uint8_t get_key(const char *name);

   public:
    bool add_key(const char *name);
    bool get(uint8_t id, MultiVar &var);
    bool get(const char *name, MultiVar &var);
    bool set(uint8_t id, MultiVar &var);
    bool set(const char *name, MultiVar &var);
    bool write(void);
    bool read(void);
    uint8_t key_count(void) { return keys; }
};

extern Config config;
