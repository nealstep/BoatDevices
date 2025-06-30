#include "m_config.hpp"

#include <LittleFS.h>

extern void cmdReply(bool replyMQTT, const char *format, ...);

// class constant definitions
const char M_Config::_defaultFilename[_filenameSize] = "/config.json";

// iniitalize globals
M_Config config;

M_Config::M_Config(const char *filename) {
    Log.traceln("M_Config init");
    if (strlcpy(_filename, filename, _filenameSize) >= _filenameSize) {
        Log.errorln("Config setup filename copy failed for: %s", filename);
    }
}

bool M_Config::exists(void) {
    bool ret;

    Log.traceln("M_Config::exists");
    LittleFS.begin();
    ret = LittleFS.exists(_filename);
    LittleFS.end();
    return ret;
}

bool M_Config::read(void) {
    bool ret = true;
    JsonDocument jDoc;

    Log.traceln("M_Config::read");
    LittleFS.begin();
    File configFile = LittleFS.open(_filename, "r");
    if (configFile) {
        auto err = deserializeJson(jDoc, configFile);
        if (err) {
            Log.errorln("Config read deserialize failed: %s", err.c_str());
            ret = false;
        }
        configFile.close();
    } else {
        Log.errorln("Failed to open config file for reading");
        ret = false;
    }
    LittleFS.end();
    if (!ret) return false;
    for (uint8_t i = 0; i < _configCount; i++) {
        if (!jDoc[_config[i].name].is<JsonVariant>()) {
            Log.warning("Config read key not found: %s", _config[i].name);
            ret = false;
        }
        switch (_config[i].type) {
            case CHARS:
                if (jDoc[_config[i].name].is<const char *>()) {
                    if (strlcpy(_config[i].datum.chars, jDoc[_config[i].name],
                                _config[i].size) >= _config[i].size) {
                        Log.errorln("Config read copy failed for: %s",
                                    _config[i].name);
                        ret = false;
                    }
                } else {
                    Log.warning("Config read bad type not chars: %s",
                                _config[i].name);
                    ret = false;
                }
                break;
            case U16:
                if (jDoc[_config[i].name].is<uint16_t>())
                    _config[i].datum.u16 = jDoc[_config[i].name];
                else {
                    Log.warning("Config read bad type not u16: %s",
                                _config[i].name);
                    ret = false;
                }
                break;
            case I16:
                if (jDoc[_config[i].name].is<int16_t>())
                    _config[i].datum.i16 = jDoc[_config[i].name];
                else {
                    Log.warning("Config read bad type not i16: %s",
                                _config[i].name);
                    ret = false;
                }
                break;
            case BOOL:
                if (jDoc[_config[i].name].is<bool>())
                    _config[i].datum.b = jDoc[_config[i].name];
                else {
                    Log.warning("Config read bad type not bool: %s",
                                _config[i].name);
                    ret = false;
                }
                break;
            default:
                Log.errorln("Config read Unexpected type for %s: %d",
                            _config[i].name, _config[i].type);
                ret = false;
        }
    }
    return ret;
}

bool M_Config::write(void) {
    bool ret = true;
    JsonDocument jDoc;

    Log.traceln("M_Config::write");
    for (uint8_t i = 0; i < _configCount; i++) {
        switch (_config[i].type) {
            case CHARS:
                jDoc[_config[i].name] = _config[i].datum.chars;
                break;
            case U16:
                jDoc[_config[i].name] = _config[i].datum.u16;
                break;
            case I16:
                jDoc[_config[i].name] = _config[i].datum.i16;
                break;
            case BOOL:
                jDoc[_config[i].name] = _config[i].datum.b;
                break;
            default:
                Log.errorln("Config write unexpected type for %s: %d",
                            _config[i].name, _config[i].type);
                ret = false;
        }
    }
    LittleFS.begin();
    File configFile = LittleFS.open(_filename, "w");
    if (configFile) {
        if (serializeJson(jDoc, configFile) == 0) {
            Log.errorln("Config write serialize failed");
            ret = false;
        }
        configFile.close();
    } else {
        Log.errorln("Config write failed");
        ret = false;
    }
    LittleFS.end();
    return ret;
}

const char *M_Config::getName(uint8_t id) {
    if (id < _configCount) {
        return _config[id].name;
    }
    return nullptr;
}

uint8_t M_Config::find(const char *name, DataTypeCodes type) {
    uint8_t ret = _badItem;
    bool found = false;

    // Log.traceln("M_Config::find");
    for (uint8_t i = 0; i < _configCount; i++) {
        if (strncmp(_config[i].name, name, _nameSize) == 0) {
            found = true;
            if (type != ANY) {
                if (_config[i].type != type) {
                    Log.errorln("Config find item %s wrong type: %d not %d",
                                name, _config[i].type, type);
                    break;
                }
            }
            ret = i;
            break;
        }
    }
    if ((!found) && (type != ANY)) {
        Log.errorln("Config find item not found: %s", name);
    }
    return ret;
}

bool M_Config::add(const char *name, DataTypeCodes type, uint8_t size) {
    uint8_t item;

    Log.traceln("M_Config::add");
    item = find(name, ANY);
    if (item != _badItem) {
        Log.warning("Config add name already exists: %s", name);
        return false;
    }
    if (_configCount >= _items) {
        Log.errorln("Config add full while adding %s", name);
        return false;
    }
    item = _configCount;
    if (strlcpy(_config[item].name, name, _nameSize) >= _nameSize) {
        Log.warning("Config add name too long max: %s", name);
        return false;
    }
    _config[item].type = type;
    _config[item].size = size;
    if (type == CHARS) {
        _config[item].datum.chars = new char[size];
        if (!_config[item].datum.chars) {
            Log.errorln("Config add New failed for: %s", name);
            return false;
        }
    }
    _configCount++;
    return true;
}

M_Config::DataTypeCodes M_Config::getType(uint8_t item) {
    // Log.traceln("M_Config::getType item");
    if (item >= _items) return ANY;
    return _config[item].type;
}

M_Config::DataTypeCodes M_Config::getType(const char *name) {
    uint8_t item;

    // Log.traceln("M_Config::getType name");
    item = find(name, ANY);
    return getType(item);
}

bool M_Config::get(uint8_t item, const char *&datum) {
    // Log.traceln("M_Config::get chars item");
    if (item >= _items) return false;
    datum = _config[item].datum.chars;
    return true;
}

bool M_Config::get(uint8_t item, uint16_t &datum) {
    // Log.traceln("M_Config::get u16 item");
    if (item >= _items) return false;
    datum = _config[item].datum.u16;
    return true;
}

bool M_Config::get(uint8_t item, int16_t &datum) {
    // Log.traceln("M_Config::get i16 item");
    if (item >= _items) return false;
    datum = _config[item].datum.i16;
    return true;
}

bool M_Config::get(uint8_t item, bool &datum) {
    // Log.traceln("M_Config::get i16 item");
    if (item >= _items) return false;
    datum = _config[item].datum.b;
    return true;
}

bool M_Config::get(const char *name, const char *&datum) {
    uint8_t item;
    // Log.traceln("M_Config::get chars name");
    item = find(name, CHARS);
    return get(item, datum);
}

bool M_Config::get(const char *name, uint16_t &datum) {
    uint8_t item;

    // Log.traceln("M_Config::get u16 name");
    item = find(name, U16);
    return get(item, datum);
}

bool M_Config::get(const char *name, int16_t &datum) {
    uint8_t item;

    // Log.traceln("M_Config::get i16 name");
    item = find(name, I16);
    return get(item, datum);
}

bool M_Config::get(const char *name, bool &datum) {
    uint8_t item;

    // Log.traceln("M_Config::get i16 name");
    item = find(name, BOOL);
    return get(item, datum);
}

bool M_Config::set(const char *name, const char *datum) {
    uint8_t item;

    Log.traceln("M_Config::set chars");
    item = find(name, CHARS);
    if (item == _badItem) return false;
    if (strlcpy(_config[item].datum.chars, datum, _config[item].size) >=
        _config[item].size) {
        Log.errorln("Config set chars for %s failed", name);
        return false;
    }
    return true;
}

bool M_Config::set(const char *name, const uint16_t &datum) {
    uint8_t item;

    Log.traceln("M_Config::set u16");
    item = find(name, U16);
    if (item == _badItem) return false;
    _config[item].datum.u16 = datum;
    return true;
}

bool M_Config::set(const char *name, const int16_t &datum) {
    uint8_t item;

    Log.traceln("M_Config::set i16");
    item = find(name, I16);
    if (item == _badItem) return false;
    _config[item].datum.i16 = datum;
    return true;
}

bool M_Config::set(const char *name, const bool &datum) {
    uint8_t item;

    Log.traceln("M_Config::set i16");
    item = find(name, BOOL);
    if (item == _badItem) return false;
    _config[item].datum.b = datum;
    return true;
}

bool cmdConfig(const char *cmd, uint8_t len, bool replyMQTT) {
    bool ret = true;
    uint8_t i = 0;

    Log.traceln("cmdConfig");
    if (len > 0) {
        switch (cmd[0]) {
            case 'l':
                Log.noticeln("Running Cmd: C%s", cmd);
                const char *name;
                while (true) {
                    name = config.getName(i);
                    if (!name) break;
                    cmdReply(replyMQTT, "Name %d: %s", i, name);
                }
                cmdReply(replyMQTT, "Name list done");
                break;
            case 'g':
                if (len < 3) {
                    ret = false;
                    Log.warning("Command not long enough: C%s", cmd);
                    cmdReply(replyMQTT, "Command not long enough");
                } else {
                    Log.noticeln("Running Cmd: C%s", cmd);
                    // ret = cmdGetConfig(&cmd[2]);
                }
                break;
            case 's':
                if (len < 5) {
                    ret = false;
                    Log.warning("Command not long enough: C%s", cmd);
                    cmdReply(replyMQTT, "Command not long enough");
                } else {
                    Log.noticeln("Running Cmd: C%s", cmd);
                    // auto value = splitString(&cmd[2]);
                    // cmdSetConfig(&cmd[2], value);
                }
                break;
            case 'W':
                ret = config.write();
                if (ret) {
                    Log.noticeln("Write succeeded");
                    cmdReply(replyMQTT, "Write succeeded");
                } else {
                    Log.errorln("Write failed");
                    cmdReply(replyMQTT, "Write failed");
                }
                break;
            case 'R':
                ret = config.read();
                if (ret) {
                    Log.noticeln("Read succeeded");
                    cmdReply(replyMQTT, "Read succeeded");
                } else {
                    Log.errorln("Read failed");
                    cmdReply(replyMQTT, "Read failed");
                }
                break;
            default:
                Log.warningln("Unknown config cmd: C%s", cmd);
                cmdReply(replyMQTT, "Unknown config cmd: C%s", cmd);
        }
    } else {
        Log.errorln("No config command");
        cmdReply(replyMQTT, "No config command");
    }
    ret = false;
    return ret;
}
