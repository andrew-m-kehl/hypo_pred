#include "PCRewriter.hpp"

namespace Stle {

static ExprPtr makeOnForPCMonitor(ExprSet &exprSet, const ClosedIntvl &intvl, const ExprPtr &arg, const ExprPtr *onExpr);

static ExprPtr makeUntilForPCMonitor(ExprSet &exprSet, const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2, bool rewriteArg2, const ExprPtr *untilExpr);

ExprPtr PCRewriter::visitConst(const ExprPtr &expr, const ConstExpr*) {
    return expr;
}

ExprPtr PCRewriter::visitBoolConst(const ExprPtr &expr, const BoolConstExpr *) {
    return expr;
}

ExprPtr PCRewriter::visitVar(const ExprPtr &expr, const VarExpr*) {
    return expr;
}

ExprPtr PCRewriter::visitApply(const ExprPtr &expr, const ApplyExpr *applyExpr) {
    int size = applyExpr->size();
    std::vector<ExprPtr> rewrittenArgs(static_cast<size_t>(size));
    bool hasRewrittenArgs = false;
    for (int i = 0; i < size; ++i) {
        ExprPtr rewrittenArg = rewriteForPCMonitor(*_exprSet, applyExpr->at(i));
        if (rewrittenArg != applyExpr->at(i))
            hasRewrittenArgs = true;
        rewrittenArgs[static_cast<size_t>(i)] = rewrittenArg;
    }
    
    if (hasRewrittenArgs)
        return _exprSet->makeApply(applyExpr->op(), rewrittenArgs.begin(), rewrittenArgs.end());
    else
        return expr;
}

ExprPtr PCRewriter::visitOn(const ExprPtr &expr, const OnExpr *onExpr) {
    return makeOnForPCMonitor(*_exprSet, onExpr->intvl(), onExpr->arg(), &expr);
}

ExprPtr PCRewriter::visitAggregate(const ExprPtr &expr, const AggregateExpr *aggregateExpr) {
    ExprPtr rewrittenArg = rewriteForPCMonitor(*_exprSet, aggregateExpr->arg());
    if (rewrittenArg != aggregateExpr->arg())
        return _exprSet->makeAggregate(aggregateExpr->op(), rewrittenArg);
    else
        return expr;
}

ExprPtr PCRewriter::visitUntil(const ExprPtr &expr, const UntilExpr *untilExpr) {
    return makeUntilForPCMonitor(*_exprSet, untilExpr->intvl(), untilExpr->defaultValue(), untilExpr->arg1(), untilExpr->arg2(), true, &expr);
}

ExprPtr PCRewriter::visitGet(const ExprPtr &expr, const GetExpr *getExpr) {
    ExprPtr rewrittenArg = rewriteForPCMonitor(*_exprSet, getExpr->arg());
    if (rewrittenArg != getExpr->arg())
        return _exprSet->makeGet(rewrittenArg);
    else
        return expr;
}

ExprPtr PCRewriter::visitLookup(const ExprPtr &expr, const LookupExpr *lookupExpr) {
    ExprPtr rewrittenArg = rewriteForPCMonitor(*_exprSet, lookupExpr->arg());
    if (rewrittenArg != lookupExpr->arg())
        return _exprSet->makeLookup(lookupExpr->delay(), lookupExpr->defaultValue(), rewrittenArg);
    else
        return expr;
}

ExprPtr PCRewriter::visitTemporal(const ExprPtr &expr, const TemporalExpr *temporalExpr) {
    const ExprPtr &arg = temporalExpr->arg();
    const ClosedIntvl &intvl = temporalExpr->intvl();
    TemporalOp temporalOp = temporalExpr->op();
    
    if (intvl.start() < Time(0) && intvl.end() > Time(0)) {
        // Rewriting "(F (-a b) e)" into "(or (F (-a 0) e) (F (0 b) e))", and similarly for G.
        ExprPtr argRewritten = rewriteForPCMonitor(*_exprSet, arg);
        // At this point, 'arg' is not an 'apply', so only 'arg' needs to be rewritten, and not the containing 'On'.
        ExprPtr temporal1 = _exprSet->makeTemporal(ClosedIntvl(intvl.start(), Time(0)), temporalOp, argRewritten);
        ExprPtr temporal2 = _exprSet->makeTemporal(ClosedIntvl(Time(0), intvl.end()), temporalOp, argRewritten);
        ApplyOp logicOp = APPLY_NOP;
        switch (temporalOp) {
        case TEMPORAL_F:
            logicOp = APPLY_BOOL_OR;
            break;            
        case TEMPORAL_G:
            logicOp = APPLY_BOOL_AND;
            break;            
        default:
            throw std::runtime_error("Unknown aggregate operation.");
        }
        return _exprSet->makeApply(logicOp, {temporal1, temporal2});
    }
    else {
        // "F/G[a,b] e" where [a,b] are both in the past or both in the future.
        ExprPtr argRewritten = rewriteForPCMonitor(*_exprSet, arg);
        if (argRewritten == arg)
            return expr;
        else
            return _exprSet->makeTemporal(intvl, temporalOp, argRewritten);
    }
}

ExprPtr PCRewriter::visitTemporalUntil(const ExprPtr &expr, const TemporalUntilExpr *temporalUntilExpr) {
    const ClosedIntvl &intvl = temporalUntilExpr->intvl();
    if (intvl.start() < Time(0) && intvl.end() > Time(0))
        throw std::runtime_error("Until look only in the future or only in the past.");
    
    const ExprPtr arg1Rewritten = rewriteForPCMonitor(*_exprSet, temporalUntilExpr->arg1());
    const ExprPtr arg2Rewritten = rewriteForPCMonitor(*_exprSet, temporalUntilExpr->arg2());    
    
    if (intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero()) {
        // All of the expressions below do not need to be rewritten.
        const ExprPtr notF = _exprSet->makeApply(APPLY_BOOL_NOT, _exprSet->makeTemporal(intvl, TEMPORAL_F, arg2Rewritten));
        ExprPtr elseExpr = _exprSet->makeTemporalUntil(intvl.future() ? ClosedIntvl::zeroToInf() : ClosedIntvl::minusInfToZero(),
                                                       arg1Rewritten, arg2Rewritten);
        if (intvl.start() > Time(0) || intvl.end() < Time(0))
            elseExpr = _exprSet->makeTemporal(intvl.future() ? ClosedIntvl(Time(0), intvl.start()) : ClosedIntvl(intvl.end(), Time(0)),
                                              TEMPORAL_G, elseExpr);
        return _exprSet->makeApply(APPLY_ITE, {notF, _exprSet->makeBoolConst(false), elseExpr});
    }
    else {
         if (arg1Rewritten == temporalUntilExpr->arg1() && arg2Rewritten == temporalUntilExpr->arg2())
             return expr;
         else
             return _exprSet->makeTemporalUntil(intvl, arg1Rewritten, arg2Rewritten);
    }
}

ExprPtr makeOnForPCMonitor(ExprSet &exprSet, const ClosedIntvl &intvl, const ExprPtr &arg, const ExprPtr *onExpr) {
    if (arg->type() == EXPR_APPLY) {
        // Rewriting "On[a,b] (f op1 op2 ...)" into "f (On[a,b] op1) (On[a,b] op2) ...".
        const ApplyExpr *argApply = static_cast<const ApplyExpr*>(arg.get());        
        int size = argApply->size();
        std::vector<ExprPtr> newArgs(static_cast<size_t>(size));
        for (int i = 0; i < size; ++i)            
            newArgs[static_cast<size_t>(i)] = makeOnForPCMonitor(exprSet, intvl, argApply->at(i), nullptr);
        
        return exprSet.makeApply(argApply->op(), newArgs.begin(), newArgs.end());
    }
    else if (arg->type() == EXPR_AGGREGATE && intvl.start() < Time(0) && intvl.end() > Time(0)) {
        // Rewriting "On[-a,b] Min op" into "min {On[-a,0] Min op, On[0,b] Min op}", similarly for Max.
        const AggregateExpr *argAggregate = static_cast<const AggregateExpr *>(arg.get());
        ExprPtr argRewritten = rewriteForPCMonitor(exprSet, arg);
        // At this point, 'arg' is not an 'apply', so only 'arg' needs to be rewritten, and not the containing 'On'.
        ExprPtr on1 = exprSet.makeOn(ClosedIntvl(intvl.start(), Time(0)), argRewritten);
        ExprPtr on2 = exprSet.makeOn(ClosedIntvl(Time(0), intvl.end()), argRewritten);
        ApplyOp op = APPLY_NOP;
        switch (argAggregate->op()) {
        case AGGREGATE_MIN:
            op = APPLY_MIN;
            break;            
        case AGGREGATE_MAX:
            op = APPLY_MAX;
            break;
        case AGGREGATE_INT:
            op = APPLY_INT;
            break;
        default:
            throw std::runtime_error("Unknown aggregate operation.");
        }
        return exprSet.makeApply(op, {on1, on2});
    }
    else {
        // "On[a,b] op" where [a,b] are both in the past or both in the future.
        ExprPtr argRewritten = rewriteForPCMonitor(exprSet, arg);
        if (onExpr && argRewritten == arg)
            return *onExpr;
        else
            return exprSet.makeOn(intvl, argRewritten);
    }
}

ExprPtr makeUntilForPCMonitor(ExprSet &exprSet, const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2, bool rewriteArg2, const ExprPtr *untilExpr) {
    if (intvl.start() < Time(0) && intvl.end() > Time(0))
        throw std::runtime_error("Interval of Until should be fully in the future or fully in the past.");
    
    ExprPtr arg2Rewritten = rewriteArg2 ? rewriteForPCMonitor(exprSet, arg2) : arg2;    
    if (arg1->type() == EXPR_APPLY) {
        const ApplyExpr *arg1Apply = static_cast<const ApplyExpr*>(arg1.get());        
        std::vector<ExprPtr> newArgs(static_cast<size_t>(arg1Apply->size()));
        for (int i = 0; i < arg1Apply->size(); ++i)
            newArgs[static_cast<size_t>(i)] = makeUntilForPCMonitor(exprSet, intvl, defaultValue, arg1Apply->at(i), arg2Rewritten, false, nullptr);
        
        return exprSet.makeApply(arg1Apply->op(), newArgs.begin(), newArgs.end());
    }
    else if (arg1->type() == EXPR_AGGREGATE && intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero()) {        
        ExprPtr arg1Rewritten = rewriteForPCMonitor(exprSet, arg1);
        // All of the expressions below do not need to be rewritten.
        ExprPtr notF = exprSet.makeApply(APPLY_BOOL_NOT, exprSet.makeTemporal(intvl, TEMPORAL_F, arg2Rewritten));
        ExprPtr elseExpr = exprSet.makeUntil(intvl.future() ? ClosedIntvl::zeroToInf() : ClosedIntvl::minusInfToZero(),
                                             defaultValue, arg1Rewritten, arg2Rewritten);
        if (intvl.start() > Time(0) || intvl.end() < Time(0)) {
            const AggregateExpr *arg1Aggregate = static_cast<const AggregateExpr *>(arg1.get());
            elseExpr = exprSet.makeOn(intvl.future() ? ClosedIntvl(Time(0), intvl.start()) : ClosedIntvl(intvl.end(), Time(0)),
                                      exprSet.makeAggregate(arg1Aggregate->op(), elseExpr));
        }
        return exprSet.makeApply(APPLY_ITE, {notF, exprSet.makeConst(defaultValue), elseExpr});
    }
    else if (arg1->type() == EXPR_GET && intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero()) {               
        ExprPtr arg1Rewritten = rewriteForPCMonitor(exprSet, arg1);
        // All of the expressions below do not need to be rewritten.
        ExprPtr notF = exprSet.makeApply(APPLY_BOOL_NOT, exprSet.makeTemporal(intvl, TEMPORAL_F, arg2Rewritten));
        ExprPtr elseExpr = exprSet.makeUntil(intvl.future() ? ClosedIntvl::zeroToInf() : ClosedIntvl::minusInfToZero(),
                                             defaultValue, arg1Rewritten, arg2Rewritten);
        if (intvl.start() > Time(0) || intvl.end() < Time(0))
            elseExpr = exprSet.makeLookup(intvl.future() ? intvl.start() : intvl.end(), defaultValue, elseExpr);
        
        return exprSet.makeApply(APPLY_ITE, {notF, exprSet.makeConst(defaultValue), elseExpr});
    }
    else {        
        // Untimed until. Only need to rewrite the first argument (second already rewritten above).
        ExprPtr arg1Rewritten = rewriteForPCMonitor(exprSet, arg1);
        if (untilExpr && (arg1Rewritten == arg1) && (arg2Rewritten == arg2))
            return *untilExpr;
        return exprSet.makeUntil(intvl, defaultValue, arg1Rewritten, arg2Rewritten);
    }
}

ExprPtr rewriteForPCMonitor(ExprSet &exprSet, const ExprPtr &expr) {
    PCRewriter visitor(exprSet);
    return visitor.visit(expr);
}

}
