#ifndef STLE_EXPR_ERROR_HPP
#define STLE_EXPR_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace Stle {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class STLE_API ExprError : public std::runtime_error {
public:
    explicit ExprError(const char *msg) : std::runtime_error(msg) {        
    }
    
    explicit ExprError(const std::string &msg) : std::runtime_error(msg) {        
    }
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif

}

#endif
