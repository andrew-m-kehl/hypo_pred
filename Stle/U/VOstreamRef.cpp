#include "VOstreamRef.hpp"

#include "StrPrinter.hpp"

namespace U {

int VOstreamRef::putc(char c) {
    *_os << c;
    if (_os->bad())
        return EOF;
    return c;
}

int VOstreamRef::puts(const char *str) {
    *_os << str;
    if (_os->bad())
        return EOF;
    return 1;
}

int VOstreamRef::puts(const std::string &str) {
    *_os << str;
    if (_os->bad())
        return EOF;
    return 1;
}

int VOstreamRef::vprintf(const char *format, va_list va) {
    StrPrinter sp;
    sp.vprintf(format, va);
    // TODO: An overload that accepts length.
    return puts(sp.str());
}

void VOstreamRef::flush() {
    _os->flush();
}

}
