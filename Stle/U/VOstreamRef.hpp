#ifndef U_V_OSTREAM_REF_HPP
#define U_V_OSTREAM_REF_HPP

#include "Api.hpp"

#include "VPrinter.hpp"

#include <ostream>

namespace U {

class U_API VOstreamRef : public VPrinter {
public:
    explicit VOstreamRef(std::ostream &os) : _os(&os) {
    }
    
    int putc(char c) override;
    
    int puts(const char *str) override;
    
    int puts(const std::string &str) override;
    
    int vprintf(const char *format, va_list va) override;
    
    void flush() override;
    
private:
    std::ostream *_os;
};

}

#endif
