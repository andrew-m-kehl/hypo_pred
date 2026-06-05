#include "PCMonitorVisitor.hpp"

#include "PCMonitorVisitorU.hpp"

namespace Stle {

PCSeries PCMonitorVisitor::visitConst(const ConstExpr *expr) {
    const PCSignal &input = _monitor->input();
    return PCSeries(input.endTime(), input.closed(), expr->value());
}

PCSeries PCMonitorVisitor::visitBoolConst(const BoolConstExpr *expr) {
    const PCSignal &input = _monitor->input();
    return PCSeries(input.endTime(), input.closed(), spaceFromBool(expr->value()));
}

PCSeries PCMonitorVisitor::visitApply(const ApplyExpr *expr) {
    int count = expr->size();
    // TODO: Perhaps use a stack array when count is small (would usually be the case).
    std::vector<const PCSeries*> argSeries(static_cast<size_t>(count), nullptr);
    for (int i = 0; i < count; ++i)
        argSeries[static_cast<size_t>(i)] = &_monitor->makeOutputUnchecked(expr->at(i));    
    return applyOpR(expr->op(), _monitor->input(), argSeries.begin(), argSeries.end());
}

PCSeries PCMonitorVisitor::visitOn(const OnExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();
    
    if (intvl.start() < Time(0) && intvl.end() > Time(0))
        throw std::runtime_error("On expression should be rewritten to look only in the future or in the past.");
    
    const ExprPtr &arg = expr->arg();
    
    if (arg->type() != EXPR_AGGREGATE)
        throw std::runtime_error("On expression should be rewritten to have AggregateExpr as the argument.");
    
    const AggregateExpr *aggregateArg = static_cast<const AggregateExpr*>(arg.get());
    const ExprPtr &subArg = aggregateArg->arg();

    return callWithIntvlSeries(_monitor, intvl, subArg, [&](const ClosedIntvl &intvl, const PCSeries &subArgSeries) {
        switch(aggregateArg->op()) {
        case AGGREGATE_MIN:
            return PCSeries::backshift(subArgSeries, intvl, SpaceMin{}, Space::inf());
    
        case AGGREGATE_MAX:
            return PCSeries::backshift(subArgSeries, intvl, SpaceMax{}, Space::minusInf());

        case AGGREGATE_INT:
        {
            Time step = (subArgSeries.endTime() - subArgSeries.startTime(0)) / Time(subArgSeries.size() - 1);
            PCSeries backStart = PCSeries::backshift(subArgSeries, intvl.start(), Space(0));
            PCSeries integral = PCSeries::apply1(SpaceIntegral{step}, backStart);
            PCSeries backEnd = PCSeries::backshift(integral, intvl.end() - intvl.start(), Space(0));
            // PCSeries result = PCSeries::apply2(SpaceMinus2{}, backEnd, integral);
            return backEnd;
        }
        default:
            throw std::runtime_error("Unsupported aggregate operator.");
        }
    });    
}

PCSeries PCMonitorVisitor::visitLookup(const LookupExpr *expr) {
    return callWithDelaySeries(_monitor, expr->delay(), expr->arg(), [&](Time delay, const PCSeries &argSeries) {
        return PCSeries::backshift(argSeries, delay, expr->defaultValue());    
    });
}

PCSeries PCMonitorVisitor::visitTemporal(const TemporalExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();
    
    if (intvl.start() < Time(0) && intvl.end() > Time(0))
        throw std::runtime_error("Temporal expression should be rewritten to look only in the future or only in the past.");
      
    return callWithIntvlSeries(_monitor, intvl, expr->arg(), [&](const ClosedIntvl &intvl, const PCSeries &argSeries) {
        switch (expr->op()) {
        case TEMPORAL_F:
            return PCSeries::backshift(argSeries, intvl, SpaceTo01{}, SpaceOr{}, Space(0));
    
        case TEMPORAL_G:
            return PCSeries::backshift(argSeries, intvl, SpaceTo01{}, SpaceAnd{}, Space(1));
    
        default:
            throw std::runtime_error("Unsupported temporal operator.");
        }    
    });    
}

PCSeries PCMonitorVisitor::visitUntil(const UntilExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();
    
    if (intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero())
        throw std::runtime_error("Timed until should be rewritten into untimed until.");

    const ExprPtr &arg1 = expr->arg1();
    if (arg1->type() == EXPR_AGGREGATE) {
        const AggregateExpr *aggregateArg1 = static_cast<const AggregateExpr*>(arg1.get());
        
        return callWithIntvlSeries2(_monitor, intvl, aggregateArg1->arg(), expr->arg2(), [&](const ClosedIntvl&, const PCSeries &subArg1Series, const PCSeries &arg2Series) {
            switch(aggregateArg1->op()) {
            case AGGREGATE_MIN:
                return untimedUntil(subArg1Series, arg2Series, SpaceMin{}, expr->defaultValue());
    
            case AGGREGATE_MAX:
                return untimedUntil(subArg1Series, arg2Series, SpaceMax{}, expr->defaultValue());

            case AGGREGATE_INT:
            {
                Time step_1 = (subArg1Series.endTime() - subArg1Series.startTime(0)) / Time(subArg1Series.size() - 1);
                Time step_2 = (arg2Series.endTime() - arg2Series.startTime(0)) / Time(arg2Series.size() - 1);
                PCSeries integArg1 = PCSeries::apply1(SpaceIntegral{step_1}, subArg1Series);
                PCSeries integArg2 = PCSeries::apply1(SpaceIntegral{step_2}, arg2Series);
                return untimedUntil(integArg1, integArg2, SpaceMin{}, expr->defaultValue());
            }
            default:
                throw std::runtime_error("Unsupported aggregate operator.");
            }    
        });
    }
    else if (arg1->type() == EXPR_GET) {
        const GetExpr *getArg1 = static_cast<const GetExpr*>(arg1.get());
        
        return callWithIntvlSeries2(_monitor, intvl, getArg1->arg(), expr->arg2(), [&](const ClosedIntvl&, const PCSeries &subArg1Series, const PCSeries &arg2Series) {
            return untimedUntil(subArg1Series, arg2Series, SpaceFirst{}, expr->defaultValue());    
        });        
    }
    else
        throw std::runtime_error("First argument of Until should be rewritten into a Min, Max, or Get expression.");
}

PCSeries PCMonitorVisitor::visitTemporalUntil(const TemporalUntilExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();

    if (intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero())
        throw std::runtime_error("Timed until should be rewritten into untimed until.");
     
    return callWithIntvlSeries2(_monitor, intvl, expr->arg1(), expr->arg2(), [&](const ClosedIntvl&, const PCSeries &arg1Series, const PCSeries &arg2Series) {
        return untimedUntil(arg1Series, arg2Series, SpaceTo01{}, SpaceMin{}, Space(0));    
    });
}

PCSeries PCMonitorVisitor::visitGet(const GetExpr*) {
    throw std::runtime_error("Should be unreachable. Aggregate expression should be handled while evaluating Until.");
}

PCSeries PCMonitorVisitor::visitVar(const VarExpr*) {
    throw std::runtime_error("Should be unreachable. Variable expression should be evaluated by the Monitor directly.");
}

PCSeries PCMonitorVisitor::visitAggregate(const AggregateExpr*) {
    throw std::runtime_error("Should be unreachable. Aggregate expression should be handled while evaluating OnExpr and UntilExpr.");
}

}
