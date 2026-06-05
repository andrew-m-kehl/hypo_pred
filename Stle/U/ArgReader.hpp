#ifndef U_ARG_READER_HPP
#define U_ARG_READER_HPP

#include "Api.hpp"

#include "ArgError.hpp"

#include <ostream>
#include <string>
#include <sstream>

namespace U {

enum ArgToken {
    ARG_EMPTY,
    ARG_KEY,
    ARG_VALUE,
    ARG_ERROR,
    ARG_EOF    
};

template <typename TPrinter>
void printArgToken(TPrinter &p, ArgToken token) {
    switch (token) {
    case ARG_EMPTY:
        p << "Empty";
        break;
        
    case ARG_ERROR:
        p << "Error";
        break;
        
    case ARG_KEY:
        p << "Key";
        break;
        
    case ARG_VALUE:
        p << "Value";
        break;
        
    case ARG_EOF:
        p << "End of List";
        break;
        
    default:
        throw std::runtime_error("Unknow argument type.");
    }       
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, ArgToken token) {
    printArgToken(p, token);
    return p;
}

U_API std::ostream &operator<<(std::ostream &os, ArgToken token);

class U_API ArgReader {
public:
    ArgReader(int argc, char **argv) : _argc(argc), _argv(argv), _argI(-1), _token(ARG_EMPTY) {        
    }
    
    ArgToken token() const {
        return _token;
    }
    
    const char *str() const {
        if (_argI <  0 || _argI >= _argc)
            return "";
        return _argv[_argI];
    }
    
    ArgToken read(bool allowKey = true);
    
    int readInt();
    double readDouble();
    const char *readStr();
    
private:
    static void argToStream(std::ostream &os, ArgToken token, const char *str);
    
    int _argc;
    char **_argv;
    int _argI;
    ArgToken _token;
};

}

#endif
