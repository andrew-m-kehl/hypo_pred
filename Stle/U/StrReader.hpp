#ifndef U_STR_READER_HPP
#define U_STR_READER_HPP

#include "Api.hpp"

#include <cstring>
#include <climits>
#include <stdexcept>

namespace U {

class U_API StrReader {
public:
    // TODO: Constructor that allows to specify length or end.
    explicit StrReader(const char *buf);    
    
    int getc();
    
    int ungetc(int c);
    
    int i() const {
        return _i;
    }
    
private:
    const char *_buf;
    int _len;
    int _i;
    char _next;
    bool _eof;
};

}

#endif
