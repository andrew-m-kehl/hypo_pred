#include "UntilExpr.hpp"
#include "ExprError.hpp"

namespace Stle {

UntilExpr::UntilExpr(const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2)
    : Expr(EXPR_UNTIL, EXPR_KIND_POINT), _intvl(intvl), _defaultValue(defaultValue), _arg1(arg1), _arg2(arg2) {
    if ((intvl.start() < Time(0) && intvl.end() > Time(0)) ||
            (intvl.start() > Time(0) && intvl.end() < Time(0)))
        throw ExprError("Cannot mix future and past in until.") ;
    if (arg1->kind() != EXPR_KIND_INTVL && arg1->kind() != EXPR_KIND_UNTIL)
        throw ExprError("Left argument should be an interval expression or a special expression allowed in the left side of until.");
    if (arg2->kind() != EXPR_KIND_POINT)
        throw ExprError("Right argument should be a point expression.");
}

void UntilExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_intvl);
    hasher.add(_defaultValue);
    hasher.add(_arg1);
    hasher.add(_arg2);
}

bool UntilExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const UntilExpr &otherUntil = static_cast<const UntilExpr&>(other);
    return
            _intvl == otherUntil._intvl &&
            _defaultValue == otherUntil._defaultValue &&
            _arg1 == otherUntil._arg1 &&
            _arg2 == otherUntil._arg2;
}

void UntilExpr::print(U::VPrinter &p) const {
    p << "(Until (" << _intvl.start() << " " << _intvl.end() << ") "
       << _defaultValue << " " << *_arg1 << " " << *_arg2 << ")";
}

}
