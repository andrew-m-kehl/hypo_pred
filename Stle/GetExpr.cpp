#include "GetExpr.hpp"

namespace Stle {

void GetExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_arg);
}

bool GetExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const GetExpr &otherGet = static_cast<const GetExpr&>(other);
    return _arg == otherGet._arg;
}

void GetExpr::print(U::VPrinter &p) const {
    p << "(Get " << *_arg << ")";
}

}
