#include "LookupExpr.hpp"

namespace Stle {

void LookupExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_delay);
    hasher.add(_defaultValue);
    hasher.add(_arg);    
}

bool LookupExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const LookupExpr &otherDelay = static_cast<const LookupExpr&>(other);
    return 
            _delay == otherDelay._delay &&
            _defaultValue == otherDelay._defaultValue &&
            _arg == otherDelay._arg;
}

void LookupExpr::print(U::VPrinter &p) const {
    p << "(Lookup " << _delay << " " << _defaultValue << " " << *_arg << ")";
}

}
