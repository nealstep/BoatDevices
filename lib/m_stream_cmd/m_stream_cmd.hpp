#pragma once

#include "all.hpp"

class StreamCmd {
   private:
    static const uint8_t _sizeCmd = mediumBufferSize;

   public:
    StreamCmd(void);
    void setStream(Stream &stream);
    bool loop(void);
    inline char *get(void) { return _cmd; }

   private:
    Stream *_stream;
    uint8_t _cmd_ind;
    char _cmd[_sizeCmd];
};
