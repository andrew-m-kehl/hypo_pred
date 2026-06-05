#include "AggregateExpr.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, AggregateOp op) {
    printAggregateOp(os, op);
    return os;
}

void AggregateExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_op);
    hasher.add(_arg);
}

bool AggregateExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const AggregateExpr &otherAgg = static_cast<const AggregateExpr&>(other);
    return _op == otherAgg._op && _arg == otherAgg._arg;
}

void AggregateExpr::print(U::VPrinter &p) const {
    p << "(" << _op << " " << *_arg << ")";
}

}
