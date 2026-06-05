#include "BoolConstExpr.hpp"

namespace Stle {

void BoolConstExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_value);
}

bool BoolConstExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const BoolConstExpr &otherConst = static_cast<const BoolConstExpr&>(other);
    return _value == otherConst._value;
}

void BoolConstExpr::print(U::VPrinter &p) const {
    if (_value)
        p << "true";
    else
        p << "false";
}

}
