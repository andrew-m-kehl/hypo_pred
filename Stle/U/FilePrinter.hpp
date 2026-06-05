#ifndef U_FILE_PRINTER_HPP
#define U_FILE_PRINTER_HPP

#include "Api.hpp"

#include "Printer.hpp"
#include "FileRef.hpp"
#include "OfFile.hpp"

#include <cstdio>

namespace U {

template <typename TFile>
class FilePrinter : public Printer<FilePrinter<TFile>> {
public:
    // TODO: This constructor will break copying/moving. Fix this.
    template<typename... TArgs>
    explicit FilePrinter(OfFileTag, TArgs&&... args) : _file(std::forward<TArgs>(args)...) {
    }
    
    int putc(char c) {
        return fputc(c, _file.file());
    }
    
    int puts(const char *str) {
        return fputs(str, _file.file());
    }    
    
    int puts(const std::string &str) {
        size_t len = str.size();
        size_t written = fwrite(str.c_str(), sizeof(char), len, _file.file());
        if (written != len)
            return EOF;
        // fputs is only required to return a non-negative int on success, emulating it here.
        return 1;
    }
    
    int vprintf(const char *format, va_list va) {
        return vfprintf(_file.file(), format, va);
    }
    
    void flush() {
        fflush(_file.file());
    }
    
private:
    TFile _file;
};

template <typename TFile>
struct IsPrinter<FilePrinter<TFile>> {
    static constexpr bool value = true;
};

typedef FilePrinter<FileRef> FileRefPrinter;

}

#endif
