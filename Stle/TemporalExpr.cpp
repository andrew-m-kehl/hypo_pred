#include "TemporalExpr.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, TemporalOp op) {
    printTemporalOp(os, op);
    return os;
}

void TemporalExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_intvl);
    hasher.add(_op);
    hasher.add(_arg);
}

bool TemporalExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const TemporalExpr &otherTemp = static_cast<const TemporalExpr&>(other);
    return _intvl == otherTemp._intvl && _op == otherTemp._op && _arg == otherTemp._arg;
}

void TemporalExpr::print(U::VPrinter &p) const {
    p << "(" << _op << " (" << _intvl.start() << " " << _intvl.end() << ") " << *_arg << ")";
}

}
