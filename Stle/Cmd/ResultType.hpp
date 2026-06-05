#ifndef STLE_CMD_RESULT_TYPE_HPP
#define STLE_CMD_RESULT_TYPE_HPP

#include "Stle/Api.hpp"

namespace Stle {
namespace Cmd {

    enum ResultType {
        RESULT_OK,        
        RESULT_ERROR,
        RESULT_SPACE,
        RESULT_STRING
    };
    
}
}

#endif
