#include "StringResult.hpp"

namespace Stle {
namespace Cmd {

bool StringResult::equals(const Result &other) const {
    if (_type != other.type())
        return false;
    const StringResult &otherString = static_cast<const StringResult&>(other);
    // _escapedValue is derived from _value. No need to compare.
    return _value == otherString.value();
}

void StringResult::print(U::VPrinter &p) const {
    p << '\"' << _escapedValue << '\"';
}

}
}
