#ifndef U_V_PRINTER_REF_HPP
#define U_V_PRINTER_REF_HPP

#include "VPrinter.hpp"

namespace U {

template <typename TPrinter>
class VPrinterRef : public VPrinter {
public:
    explicit VPrinterRef(TPrinter &printer) : _printer(&printer) {
    }

    int putc(char c) override {
        return _printer->putc(c);
    }

    int puts(const char *str) override {
        return _printer->puts(str);
    }

    int puts(const std::string &str) override {
        return _printer->puts(str);
    }

    int vprintf(const char *format, va_list va) override {
        return _printer->vprintf(format, va);
    }

    void flush() override {
        _printer->flush();
    }

private:
    TPrinter *_printer;
};

template <typename TPrinter>
struct IsPrinter<VPrinterRef<TPrinter>> {
    static constexpr bool value = true;
};

}

#endif
