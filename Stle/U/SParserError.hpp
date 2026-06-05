#ifndef U_S_PARSER_ERROR_HPP
#define U_S_PARSER_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>
#include <string>

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class U_API SParserError : public std::runtime_error {
public:
    SParserError(const char *msg) : std::runtime_error(msg) {        
    }
    
    SParserError(const std::string &msg) : std::runtime_error(msg) {        
    }
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif

}

#endif
