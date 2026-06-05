#ifndef STLE_PC_MONITOR_VISITOR_U_HPP
#define STLE_PC_MONITOR_VISITOR_U_HPP

#include "PCSeries.hpp"
#include "VarExpr.hpp"
#include "ExprPtr.hpp"
#include "ApplyOp.hpp"
#include "PCSignal.hpp"
#include "IntvlEnd.hpp"

namespace Stle {

template <typename TOpV, typename TOp2, typename TIt>
PCSeries applyR2(TOpV opV, TOp2 op2, TIt begin, TIt end) {
    size_t count = static_cast<size_t>(std::distance(begin, end));
    if (count == 2)
        return PCSeries::apply2(op2, **begin, **std::next(begin));
    return PCSeries::applyR(opV, begin, end);
}

template <typename TOpV, typename TOp1, typename TOp2, typename TIt>
PCSeries applyR12(TOpV opV, TOp1 op1, TOp2 op2, TIt begin, TIt end) {
    size_t count = static_cast<size_t>(std::distance(begin, end));
    if (count == 1)
        return PCSeries::apply1(op1, **begin);
    if (count == 2)
        return PCSeries::apply2(op2, **begin, **std::next(begin));
    return PCSeries::applyR(opV, begin, end);
}

template <typename TIt>
PCSeries applyOpR(ApplyOp op, const PCSignal &signal, TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    
    switch (op) {    
    case APPLY_NOP:
        return PCSeries(signal.endTime(), signal.closed(), Space(0));
    
    case APPLY_MIN: {
        // Case of 0 arguments has to be handled here, because PCSeries::applyR does not know
        // what should be the duration/closedness of the result.
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space::inf());
        else
            return applyR2(SpaceMinR{}, SpaceMin{}, begin, end);
    }
        
    case APPLY_MAX: {
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space::minusInf());
        else
            return applyR2(SpaceMaxR{}, SpaceMax{}, begin, end);
    }

    case APPLY_DER: {
        /* f'(x) = [f(x + h) - f(x)] / h */
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space::minusInf());
        else{
            /* TODO: Wrap the following lines in a similar way to APPLY_BOOL_NOT.
            *
            * E.g.:
            *
            * APPLY_DER: {
            * if (count != 1)
            *     throw std::runtime_error("Derivative operator should have one argument.");
            * return PCSeries::apply1(SpaceDer{step}, **begin);
            *
            */
            /* h */
            PCSeries clone = PCSeries::backshift(**begin, Time(0), Space::minusInf());
            Time step = (clone.endTime() - clone.startTime(0)) / Time(clone.size() - 1);
            PCSeries step_signal = PCSeries(signal.endTime(), signal.closed(), Space(step));
            /* f(x + h) */
            PCSeries shifted_clone = PCSeries::backshift(clone, step, Space::minusInf());
            /* f(x + h) - f(x) */
            PCSeries diff_signal = PCSeries::apply2(SpaceMinus2{}, shifted_clone, clone);
            /* [f(x + h) - f(x)] / h */
            PCSeries result = PCSeries::apply2(SpaceDiv{}, diff_signal, step_signal);

            return result;
        }
    }

    case APPLY_INT: {
        /* I(f) = sum(i = 0, i = n) [f(x_i) * h] */
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space::minusInf());
        else{
            /* TODO: Wrap the following lines in a similar way to APPLY_BOOL_NOT.
            *
            * E.g.:
            *
            * APPLY_INT: {
            * if (count != 1)
            *     throw std::runtime_error("Integral operator should have one argument.");
            * return PCSeries::apply1(SpaceIntegral{step}, **begin);
            *
            */
            /* h */
            PCSeries clone = PCSeries::backshift(**begin, Time(0), Space::minusInf());
            Time step = (clone.endTime() - clone.startTime(0)) / Time(clone.size() - 1);
            PCSeries step_signal = PCSeries(signal.endTime(), signal.closed(), Space(step));
            /* f(x_i) * h */
            PCSeries mult_signal = PCSeries::apply2(SpaceMul{}, clone, step_signal);
            /* sum(i = 0, i = n) [f(x_i) * h] */
            PCSeries result = PCSeries::apply1(SpaceAccum{}, mult_signal);

            return result;
        }
    }

    case APPLY_PLUS: {
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space(0));
        else
            return applyR2(SpacePlusR{}, SpacePlus{}, begin, end);
    }
        
    case APPLY_MINUS: {
        if (count == 0)
            throw std::runtime_error("Minus should have one or more arguments.");
        return applyR12(SpaceMinusR{}, SpaceMinus1{}, SpaceMinus2{}, begin, end);
    }
        
    case APPLY_MUL: {
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), Space(1));
        else
            return applyR2(SpaceMulR{}, SpaceMul{}, begin, end);
    }
        
    case APPLY_DIV: {
        if (count != 2)
            throw std::runtime_error("Division should have two arguments.");
        return PCSeries::apply2(SpaceDiv{}, **begin, **std::next(begin));
    }
        
    case APPLY_BOOL_AND: {
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), spaceFromBool(true));
        else
            return applyR2(SpaceAndR{}, SpaceAnd{}, begin, end);
    }
        
    case APPLY_BOOL_OR: {
        if (count == 0)
            return PCSeries(signal.endTime(), signal.closed(), spaceFromBool(true));
        else
            return applyR2(SpaceOrR{}, SpaceOr{}, begin, end);
    }
        
    case APPLY_BOOL_NOT: {
        if (count != 1)
            throw std::runtime_error("Boolean Not should have one argument.");
        return PCSeries::apply1(SpaceNot{}, **begin);
    }
        
    case APPLY_BOOL_IMPL: {
        if (count < 2)
            throw std::runtime_error("Boolean Implication should have 2 or more arguments.");
        return applyR2(SpaceImpliesR{}, SpaceImplies{}, begin, end);
    }
        
    case APPLY_ABS: {
        if (count != 1)
            throw std::runtime_error("Abs should have one argument.");
        return PCSeries::apply1(SpaceAbs{}, **begin);
    }
        
    case APPLY_ITE: {
        if (count != 3)
            throw std::runtime_error("Ite should have three arguments.");
        return PCSeries::applyR(SpaceIteR{}, begin, end);
    }
        
    case APPLY_LT: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceLtR{}, SpaceLt{}, begin, end);
    }
        
    case APPLY_LEQ: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceLeqR{}, SpaceLeq{}, begin, end);
    }
        
    case APPLY_GT: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceGtR{}, SpaceGt{}, begin, end);
    }
        
    case APPLY_GEQ: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceGeqR{}, SpaceGeq{}, begin, end);
    }
        
    default:
        throw std::runtime_error("Unsupported operator.");
    }
}

template <typename TMonitor, typename TOp>
PCSeries callWithIntvlSeries(TMonitor &_monitor, const ClosedIntvl &intvl, const ExprPtr &expr, TOp op) {        
    if (intvl.start() >= Time(0))
        return op(intvl, _monitor->makeOutputUnchecked(expr));        
    else {
        PCSeries revResult;
        
        if (expr->type() == EXPR_VAR)
            revResult = op(ClosedIntvl::neg(intvl), _monitor->makeReverse(static_cast<const VarExpr*>(expr.get())));
        else
            revResult = op(ClosedIntvl::neg(intvl), PCSeries::reverse(_monitor->makeOutputUnchecked(expr)));        
        
        return PCSeries::reverse(revResult, !_monitor->input().closed());
    }
}

template <typename TMonitor, typename TOp>
PCSeries callWithIntvlSeries2(TMonitor &_monitor, const ClosedIntvl &intvl, const ExprPtr &expr1, const ExprPtr &expr2, TOp op) {        
    if (intvl.start() >= Time(0))
        return op(intvl, _monitor->makeOutputUnchecked(expr1), _monitor->makeOutputUnchecked(expr2));        
    else {
        PCSeries revResult;
        
        if (expr1->type() == EXPR_VAR && expr2->type() == EXPR_VAR)
            revResult = op(ClosedIntvl::neg(intvl),
                           _monitor->makeReverse(static_cast<const VarExpr*>(expr1.get())),
                           _monitor->makeReverse(static_cast<const VarExpr*>(expr2.get())));
        else if (expr1->type() == EXPR_VAR) // && expr2->type() != EXPR_VAR
            revResult = op(ClosedIntvl::neg(intvl),
                           _monitor->makeReverse(static_cast<const VarExpr*>(expr1.get())),
                           PCSeries::reverse(_monitor->makeOutputUnchecked(expr2)));        
        else if (expr2->type() == EXPR_VAR) // && expr1->type() != EXPR_VAR
            revResult = op(ClosedIntvl::neg(intvl),
                           PCSeries::reverse(_monitor->makeOutputUnchecked(expr1)),
                           _monitor->makeReverse(static_cast<const VarExpr*>(expr2.get())));
        else // if (expr1->type() != EXPR_VAR && expr2->type() != EXPR_VAR)
            revResult = op(ClosedIntvl::neg(intvl),
                           PCSeries::reverse(_monitor->makeOutputUnchecked(expr1)),
                           PCSeries::reverse(_monitor->makeOutputUnchecked(expr2)));
                           
        return PCSeries::reverse(revResult, !_monitor->input().closed());
    }
}

template <typename TMonitor, typename TOp>
PCSeries callWithDelaySeries(TMonitor &_monitor, Time delay, const ExprPtr &expr, TOp op) {        
    if (delay >= Time(0))
        return op(delay, _monitor->makeOutputUnchecked(expr));        
    else {
        PCSeries revResult;
        
        if (expr->type() == EXPR_VAR)
            revResult = op(-delay, _monitor->makeReverse(static_cast<const VarExpr*>(expr.get())));
        else
            revResult = op(-delay, PCSeries::reverse(_monitor->makeOutputUnchecked(expr)));        
        
        return PCSeries::reverse(revResult, !_monitor->input().closed());
    }
}

STLE_API inline void untilRead(const PCSeries &series, int i, IntvlEnd &end, Space &value) {
    if (i >= 0) {
        PCSeg seg = series[i];
        end = IntvlEnd(seg.endTime(), series.rightClosed(i));    
        value = seg.value();
    }
    else {
        end = IntvlEnd(Time(0), false);
        value = Space(0);
    }
}

STLE_API inline void untilAdd(std::vector<PCSeg> &segments, Time endTime, Space value) {
    if (segments.empty()) {
        segments.emplace_back(endTime, value);
        return;
    }
    
    const PCSeg &last = segments[segments.size() - 1];
    if (last.value() != value || last.endTime() == endTime)
        segments.emplace_back(endTime, value);
}

}

#endif
