#include "SpaceResult.hpp"

namespace Stle {
namespace Cmd {

bool SpaceResult::equals(const Result &other) const {
    if (_type != other.type())
        return false;
    
    const SpaceResult otherSpace = static_cast<const SpaceResult&>(other);
    return _value == otherSpace._value;
}

void SpaceResult::print(U::VPrinter &p) const {
    p << _value;
}

}
}
