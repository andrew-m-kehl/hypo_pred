#include "StrReader.hpp"

namespace U {

StrReader::StrReader(const char *buf) {
    _buf = buf;
    size_t len = strlen(buf);
    if (len > INT_MAX)
        throw std::runtime_error("Input string length should be at most INT_MAX.");
    _len = static_cast<int>(len);
    _i = 0;
    _next = _buf[_i];
    _eof = false;
}

int StrReader::getc() {
    if (_i == _len) {
        _eof = true;
        return EOF;
    }
    char c = _next;
    ++_i;
    _next = _buf[_i];
    return c;
}

int StrReader::ungetc(int c) {
    if (c == EOF)
        return EOF;
    if (_i != 0)            
        --_i;
    _next = static_cast<char>(c);
    _eof = false;
    return c;
}


}
