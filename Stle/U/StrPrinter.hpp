#ifndef U_STR_PRINTER_HPP
#define U_STR_PRINTER_HPP

#include "Api.hpp"

#include "Printer.hpp"

#include <vector>
#include <cstdlib>
#include <cstdarg>

namespace U {

// TODO: Benchmark against std::stringstream.
#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class U_API StrPrinter : public Printer<StrPrinter> {
public:
    StrPrinter();
    
    const char *str() const {
        return _buf.data();
    }
   
    int putc(char c);
    
    int puts(const char *str);
    
    int puts(const std::string &str);
    
    int vprintf(const char *format, va_list va1);
    
    void flush() {
    }
    
private:
    int putsImpl(const char *str, size_t len);
    
    std::vector<char> _buf;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

template <>
struct IsPrinter<StrPrinter> {
    static constexpr bool value = true;
};

}

#endif
