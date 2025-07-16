#include "n_config.hpp"

extern Config config;

MultiVar::MultiVar(double v) {
    Var.d = v;
    type = 'd';
}

MultiVar::MultiVar(uint32_t v) {
    Var.u32 = v;
    type = 'u32';
}
MultiVar::MultiVar(int32_t v) {
    Var.i32 = v;
    type = 'i32';
}
MultiVar::MultiVar(const char *v) {
    Var.c = v;
    type = 'c';
}

MultiVar MultiVar::operator=(double v) {
    Var.d = v;
    type = 'd';
}

MultiVar MultiVar::operator=(uint32_t v) {
    Var.u32 = v;
    type = 'u32';
}

MultiVar MultiVar::operator=(int32_t v) {
    Var.i32 = v;
    type = 'i32';
}

bool MultiVar::get(double &d) {
    if (type == 'd') {
        d = Var.d;
        return true;
    } else
        return false;
};

bool MultiVar::get(uint32_t &u32) {
    if (type == 'u32') {
        u32 = Var.u32;
        return true;
    } else
        return false;
};

bool MultiVar::get(int32_t &i32) {
    if (type == 'i32') {
        i32 = Var.i32;
        return true;
    } else
        return false;
};

bool MultiVar::get(const char *(&c)) {
    if (type == 'c') {
        c = Var.c;
        return true;
    } else
        return false;
};

uint8_t Config::get_key(const char *name) { return bad_value; }

bool Config::add_key(const char *name) { return false; }

bool Config::get(uint8_t id, MultiVar &var) { return false; }

bool Config::get(const char *name, MultiVar &var) { return false; }

bool Config::set(uint8_t id, MultiVar &var) { return false; }

bool Config::set(const char *name, MultiVar &var) { return false; }

bool Config::write(void) { return false; }

bool Config::read(void) { return false; }