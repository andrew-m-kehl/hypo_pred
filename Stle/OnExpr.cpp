#include "OnExpr.hpp"

namespace Stle {

void OnExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_intvl);
    hasher.add(_arg);
}

bool OnExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const OnExpr &otherOn = static_cast<const OnExpr&>(other);
    return _intvl == otherOn._intvl && _arg == otherOn._arg;
}

void OnExpr::print(U::VPrinter &p) const {
    p << "(On (" << _intvl.start() << " " << _intvl.end() << ") " << *_arg << ")";
}

}
