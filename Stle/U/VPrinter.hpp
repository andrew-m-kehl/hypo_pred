#ifndef U_V_PRINTER_HPP
#define U_V_PRINTER_HPP

#include "Api.hpp"

#include "Printer.hpp"

namespace U {

class U_API VPrinter : public Printer<VPrinter>  {
public:
    virtual int putc(char c) = 0;
    virtual int puts(const char *str) = 0;
    virtual int puts(const std::string &str) = 0;
    virtual int vprintf(const char *format, va_list va) = 0;
    virtual void flush() = 0;
    
    virtual ~VPrinter() = default;
    // Need to re-enable moving after adding a virtual destructor.
    VPrinter(VPrinter &&) = default;
    VPrinter &operator=(VPrinter &&) = default;
    VPrinter(const VPrinter &) = default;
    VPrinter &operator=(const VPrinter &) = default;
    VPrinter() = default;
};

template <>
struct IsPrinter<VPrinter> {
    static constexpr bool value = true;
};

}

#endif
