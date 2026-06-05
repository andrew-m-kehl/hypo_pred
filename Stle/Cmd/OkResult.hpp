#ifndef STLE_CMD_OK_RESULT_HPP
#define STLE_CMD_OK_RESULT_HPP

#include "Stle/Api.hpp"

#include "Result.hpp"

namespace Stle {
namespace Cmd {

class STLE_API OkResult : public Result {
public:
    OkResult() : Result(RESULT_OK) {        
    }
    
    bool equals(const Result &other) const override;
    
    void print(U::VPrinter &p) const override;
};

}
}

#endif
