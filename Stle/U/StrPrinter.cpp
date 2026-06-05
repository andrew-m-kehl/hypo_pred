#include "StrPrinter.hpp"

#include <cstring>

namespace U {

StrPrinter::StrPrinter() {
    _buf.emplace_back('\0');
}

int StrPrinter::putc(char c) {
    _buf[_buf.size() - 1] = c;
    _buf.emplace_back('\0');
    return c;
}

int StrPrinter::puts(const char *str) {
    return putsImpl(str, strlen(str));
}

int StrPrinter::puts(const std::string &str) {
    return putsImpl(str.c_str(), str.size());
}

int StrPrinter::putsImpl(const char *str, size_t len) {
    if (len <= 0)
        return 0;
    // Note that _buf has '\0' in the end.
    size_t oldSize = _buf.size();
    _buf.resize(oldSize + len, '\0');    
#ifdef _MSC_VER
    strcpy_s(_buf.data() + oldSize - 1, len+1, str);
#else
    strcpy(_buf.data() + oldSize - 1, str);
#endif
    // fputs is only required to return a non-negative int on success, emulating it here.
    return 1;
}

int StrPrinter::vprintf(const char *format, va_list va1) {
    va_list va2;
    va_copy(va2, va1);
    int printLen = vsnprintf(nullptr, 0, format, va1);
    if (printLen <= 0)
        return 0;
    size_t oldSize = _buf.size();
    _buf.resize(oldSize + static_cast<size_t>(printLen), '\0');
    int result = vsnprintf(_buf.data() + oldSize - 1, static_cast<size_t>(printLen)+1, format, va2);
    va_end(va2);
    return result;        
}


}
