#ifndef STLE_CMD_SPACE_RESULT_HPP
#define STLE_CMD_SPACE_RESULT_HPP

#include "Stle/Api.hpp"

#include "Result.hpp"

#include "Stle/SpaceTime.hpp"

namespace Stle {
namespace Cmd {

class STLE_API SpaceResult : public Result {
public:
    explicit SpaceResult(Space value) : Result(RESULT_SPACE), _value(value) {        
    }
    
    explicit SpaceResult(double value) : SpaceResult(Space(value)) {        
    }
    
    Space value() const {
        return _value;
    }
    
    bool equals(const Result &other) const override;
    
    void print(U::VPrinter &p) const override;
    
private:
    Space _value;        
};

}
}

#endif
