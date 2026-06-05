#ifndef U_FILE_ERROR_HPP
#define U_FILE_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class U_API IOError : public std::runtime_error {
public:
    explicit IOError(const char *msg) : std::runtime_error(msg) {        
    }
    
    explicit IOError(const std::string &msg) : std::runtime_error(msg) {        
    }
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif

}

#endif
