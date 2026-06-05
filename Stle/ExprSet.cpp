#include "ExprSet.hpp"

namespace Stle {

ExprPtr ExprSet::makeConst(Space value) {    
    return make<ConstExpr>(value);
}

ExprPtr ExprSet::makeConst(int value) {
    return make<ConstExpr>(value);
}

ExprPtr ExprSet::makeConst(double value) {
    return make<ConstExpr>(value);
}

ExprPtr ExprSet::makeBoolConst(bool value) {
    return make<BoolConstExpr>(value);
}

ExprPtr ExprSet::makeVar(const char *name) {
    return make<VarExpr>(name);
}

ExprPtr ExprSet::makeApply(ApplyOp op, const ExprPtr &arg) {
    return make<ApplyExpr>(op, arg);
}

ExprPtr ExprSet::makeApply(ApplyOp op, std::initializer_list<ExprPtr> args) {    
    return make<ApplyExpr>(op, args);
}

ExprPtr ExprSet::makeOn(const ClosedIntvl &intvl, const ExprPtr &arg) {
    return make<OnExpr>(intvl, arg);
}

ExprPtr ExprSet::makeAggregate(AggregateOp op, const ExprPtr &arg) {
    return make<AggregateExpr>(op, arg);
}

ExprPtr ExprSet::makeUntil(const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2) {
    return make<UntilExpr>(intvl, defaultValue, arg1, arg2);
}

ExprPtr ExprSet::makeGet(const ExprPtr &arg) {
    return  make<GetExpr>(arg);
}

ExprPtr ExprSet::makeLookup(Time delay, Space defaultValue, const ExprPtr &arg) {
    return make<LookupExpr>(delay, defaultValue, arg);
}

ExprPtr ExprSet::makeTemporal(const ClosedIntvl &intvl, TemporalOp op, const ExprPtr &arg) {
    return make<TemporalExpr>(intvl, op, arg);
}

ExprPtr ExprSet::makeTemporalUntil(const ClosedIntvl &intvl, const ExprPtr &arg1, const ExprPtr &arg2) {
    return make<TemporalUntilExpr>(intvl, arg1, arg2);
}


}
