#ifndef U_S_LEXER_ERROR_HPP
#define U_S_LEXER_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class U_API SLexerError : public std::runtime_error {
public:
    SLexerError(const char *msg) : std::runtime_error(msg) {        
    }
    
    SLexerError(const std::string &msg) : std::runtime_error(msg) {        
    }
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif

}

#endif
