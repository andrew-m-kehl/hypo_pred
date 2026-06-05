#include "ConstExpr.hpp"

namespace Stle {

void ConstExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_value);
}

bool ConstExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const ConstExpr &otherConst = static_cast<const ConstExpr&>(other);
    return _value == otherConst._value;
}

void ConstExpr::print(U::VPrinter &p) const {
    p << _value;
}

}
