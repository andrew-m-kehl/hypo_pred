#ifndef U_ARG_NO_ERROR_HPP
#define U_ARG_NO_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace U {

class U_API ArgNoError : public std::runtime_error {
public:
    ArgNoError() : std::runtime_error("") {        
    }
    
    explicit ArgNoError(const char *msg) : std::runtime_error(msg) {        
    }
    
    explicit ArgNoError(const std::string &msg) : std::runtime_error(msg) {        
    }
};

}

#endif
