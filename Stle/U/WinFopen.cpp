#ifdef _WIN32

#include "WinU.hpp"
#include "IOError.hpp"

#include <cstdio>
#include <memory>
#include <sstream>

namespace U {

FILE *u8FOpen(const char *u8Name, const char *mode, bool nothrow = true) {
    try {
        std::unique_ptr<wchar_t[]> wName(makeWStr(u8Name));
        std::unique_ptr<wchar_t[]> wMode(makeWStr(mode));
        FILE *file = nullptr;
        int error = _wfopen_s(&file, wName.get(), wMode.get());
        if (error && !nothrow) {
            // TODO: Check error code for more informative message.
            std::stringstream ss;
            ss << "Cannot open file '" << u8Name << "', _wfopen_s error: " << error << ".";
            throw IOError(ss.str());
        }
        return file;
    }
    catch (std::exception &) {
        if (!nothrow)
            throw;
        return nullptr;
    }    
}

}

#endif
