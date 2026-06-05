#include "TemporalUntilExpr.hpp"
#include "ExprError.hpp"

namespace Stle {

TemporalUntilExpr::TemporalUntilExpr(const ClosedIntvl &intvl, const ExprPtr &arg1, const ExprPtr &arg2)    
    : Expr(EXPR_TEMPORAL_UNTIL, EXPR_KIND_POINT), _intvl(intvl), _arg1(arg1), _arg2(arg2) {
    if ((intvl.start() < Time(0) && intvl.end() > Time(0)) ||
            (intvl.start() > Time(0) && intvl.end() < Time(0)))
        // TODO: Perhaps allow mixing future and past in temporal until.
        throw ExprError("Cannot mix future and past in Until.") ;
    if (arg1->kind() != EXPR_KIND_POINT)
        throw ExprError("Left argument should be a point expression.");
    if (arg2->kind() != EXPR_KIND_POINT)
        throw ExprError("Right argument should be a point expression.");
}

void TemporalUntilExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_intvl);
    hasher.add(_arg1);
    hasher.add(_arg2);
}

bool TemporalUntilExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const TemporalUntilExpr &otherUntil = static_cast<const TemporalUntilExpr&>(other);
    return
            _intvl == otherUntil._intvl &&
            _arg1 == otherUntil._arg1 &&
            _arg2 == otherUntil._arg2;
}

void TemporalUntilExpr::print(U::VPrinter &p) const {
    p << "(StlUntil (" << _intvl.start() << " " << _intvl.end() << ") "
       << *_arg1 << " " << *_arg2 << ")";
}


}
