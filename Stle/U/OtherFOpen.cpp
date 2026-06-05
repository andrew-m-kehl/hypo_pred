#ifdef _WIN32
#else

#include "IOError.hpp"

#include <cstdio>
#include <sstream>

namespace U {

FILE *u8FOpen(const char *u8Name, const char *mode, bool nothrow = true) {
    FILE *file = fopen(u8Name, mode);
    if (!file && !nothrow) {
        // TODO: Check error code for more informative message.
        std::stringstream ss;        
        ss << "Cannot open file: '" << u8Name << "': fopen error: " << errno << ".";
        throw IOError(ss.str());
    }
    return file;        
}

}

#endif
