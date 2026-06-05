#ifndef U_ARG_ERROR_HPP
#define U_ARG_ERROR_HPP

#include "Api.hpp"

#include <stdexcept>

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4275)
#endif
class U_API ArgError : public std::runtime_error {
public:
    explicit ArgError(const char *msg, bool silent = false) : std::runtime_error(msg), _silent(silent) {        
    }
    
    explicit ArgError(const std::string &msg, bool silent = false) : std::runtime_error(msg), _silent(silent) {        
    }
    
    bool silent() const {
        return _silent;
    }
    
private:
    bool _silent;
};
#ifdef _MSC_VER
    #pragma warning(default: 4275)
#endif

}

#endif
