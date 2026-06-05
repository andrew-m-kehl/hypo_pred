#ifndef STLE_READ_SIGNAL_ERROR_HPP
#define STLE_READ_SIGNAL_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace Stle {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class STLE_API ReadSignalError : public std::runtime_error {
public:
    explicit ReadSignalError(const char *msg) : std::runtime_error(msg) {        
    }
    
    explicit ReadSignalError(const std::string &msg) : std::runtime_error(msg) {        
    }
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif
}

#endif
