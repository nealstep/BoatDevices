#include "m_stream_cmd.hpp"

StreamCmd::StreamCmd(void) {
    Log.traceln("StreamCmd Init");
    _stream = nullptr;
    _cmd[0] = '\0';
    _cmd_ind = 0;
}

void StreamCmd::setStream(Stream& stream) {
    Log.traceln("StreamCmd::set");
    _stream = &stream;
}

bool StreamCmd::loop(void) {
    bool r;
    char c;

    if (!_stream) {
        Log.error("Stream not set");
        return false;
    }
    r = false;
    // if (_stream->available())
    //     Log.traceln("StreamCmd::cmd_check data");
    while (_stream->available()) {
        c = _stream->read();
        if (c == '\r') {
            continue;
        }
        if (c =='\n') {
            _cmd[_cmd_ind++] = '\0';
            _cmd_ind = 0;
            r = true;
            break;
        } else {
            _cmd[_cmd_ind++] = c;
            if (_cmd_ind >= _sizeCmd) {
                Log.error("StreamCmd loop command too long");
            }
        }
    }
    return r; 
}
