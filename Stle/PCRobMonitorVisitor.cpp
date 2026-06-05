#include "PCRobMonitorVisitor.hpp"

#include "PCMonitorVisitorU.hpp"
#include "SpaceTimeRob.hpp"

namespace Stle {

PCSeries PCRobMonitorVisitor::visitConst(const ConstExpr *expr) {
    const PCSignal &input = _monitor->input();
    return PCSeries(input.endTime(), input.closed(), expr->value());
}

PCSeries PCRobMonitorVisitor::visitBoolConst(const BoolConstExpr *expr) {
    const PCSignal &input = _monitor->input();
    return PCSeries(input.endTime(), input.closed(), spaceRobFromBool(expr->value()));
}

PCSeries PCRobMonitorVisitor::visitApply(const ApplyExpr *expr) {
    int count = expr->size();
    // TODO: Perhaps use a stack array when count is small (would usually be the case).
    std::vector<const PCSeries*> argSeries(static_cast<size_t>(count), nullptr);
    for (int i = 0; i < count; ++i)
        argSeries[static_cast<size_t>(i)] = &_monitor->makeOutputUnchecked(expr->at(i));
    
    switch (expr->op()) {
    case APPLY_BOOL_AND: {
        if (count == 0)
            return PCSeries(_monitor->input().endTime(), _monitor->input().closed(), spaceFromBool(true));
        else
            return applyR2(SpaceMinR{}, SpaceMin{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_BOOL_OR: {
        if (count == 0)
            return PCSeries(_monitor->input().endTime(), _monitor->input().closed(), spaceFromBool(true));
        else
            return applyR2(SpaceMaxR{}, SpaceMax{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_BOOL_NOT: {
        if (count != 1)
            throw std::runtime_error("Boolean Not should have one argument.");
        return PCSeries::apply1(SpaceMinus1{}, **argSeries.begin());
    }
        
    case APPLY_BOOL_IMPL: {        
        if (count < 2)
            throw std::runtime_error("Boolean Implication should have 2 or more arguments.");
        return applyR2(SpaceImpliesRobR{}, SpaceRobImplies{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_LT: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceLtRobR{}, SpaceLtRob{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_LEQ: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceLeqRobR{}, SpaceLeqRob{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_GT: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceGtRobR{}, SpaceGtRob{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_GEQ: {
        if (count < 2)
            throw std::runtime_error("Comparison should have two or more arguments.");
        return applyR2(SpaceGeqRobR{}, SpaceGeqRob{}, argSeries.begin(), argSeries.end());
    }
        
    case APPLY_ITE: {
        if (count != 3)
            throw std::runtime_error("Ite should have three arguments.");
        return PCSeries::applyR(SpaceIteRobR{}, argSeries.begin(), argSeries.end());
    }
        
    default:
        return applyOpR(expr->op(), _monitor->input(), argSeries.begin(), argSeries.end());
    }
}

PCSeries PCRobMonitorVisitor::visitTemporal(const TemporalExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();
    
    if (intvl.start() < Time(0) && intvl.end() > Time(0))
        throw std::runtime_error("Temporal expression should be rewritten to look only in the future or only in the past.");
      
    return callWithIntvlSeries(_monitor, intvl, expr->arg(), [&](const ClosedIntvl &intvl, const PCSeries &argSeries) {
        switch (expr->op()) {
        case TEMPORAL_F:
            return PCSeries::backshift(argSeries, intvl, SpaceMax{}, Space::minusInf());
    
        case TEMPORAL_G:
            return PCSeries::backshift(argSeries, intvl, SpaceMin{}, Space::inf());
    
        default:
            throw std::runtime_error("Unsupported temporal operator.");
        }    
    });
}

PCSeries PCRobMonitorVisitor::visitTemporalUntil(const TemporalUntilExpr *expr) {
    const ClosedIntvl &intvl = expr->intvl();

    if (intvl != ClosedIntvl::zeroToInf() && intvl != ClosedIntvl::minusInfToZero())
        throw std::runtime_error("Timed until should be rewritten into untimed until.");
     
    return callWithIntvlSeries2(_monitor, intvl, expr->arg1(), expr->arg2(), [&](const ClosedIntvl&, const PCSeries &arg1Series, const PCSeries &arg2Series) {
        return untimedUntil(arg1Series, arg2Series);    
    });
}

PCSeries PCRobMonitorVisitor::untimedUntil(const PCSeries &series1, const PCSeries &series2) {
    int i1 = series1.size() - 1;
    int i2 = series2.size() - 1;
    IntvlEnd nextEnd1;
    Space nextValue1(0);
    IntvlEnd nextEnd2;    
    Space nextValue2(0);
    untilRead(series1, i1, nextEnd1, nextValue1);
    untilRead(series2, i2, nextEnd2, nextValue2);
    
    if (nextEnd1 != nextEnd2)
        throw std::runtime_error("Series should end at the same time.");
    
    std::vector<PCSeg> result;
    result.reserve(static_cast<size_t>(std::max(series1.size(), series2.size())));
    
    Space outputValue = Space::minusInf();
    // Values that participate in producing the output value.
    // Will be initialized in the first iteration.
    Space value1(0);
    Space value2(0);    
    while(i1 >= 0 || i2 >= 0) {        
        Time segmentEnd(0);
        if (nextEnd1 > nextEnd2) {
            segmentEnd = nextEnd1.end();
            value1 = nextValue1;
            --i1;
            untilRead(series1, i1, nextEnd1, nextValue1);            
        }
        else if (nextEnd1 < nextEnd2) {
            segmentEnd = nextEnd2.end();
            value2 = nextValue2;
            --i2;
            untilRead(series2, i2, nextEnd2, nextValue2);
            
        }
        else { // if (end1 == end2)            
            segmentEnd = nextEnd1.end();
            value1 = nextValue1;
            --i1;
            untilRead(series1, i1, nextEnd1, nextValue1);
            value2 = nextValue2;
            --i2;
            untilRead(series2, i2, nextEnd2, nextValue2);
        }        
        outputValue = spaceMin(value1, spaceMax(value2, outputValue));        
        untilAdd(result, segmentEnd, outputValue);
    }
    
    std::reverse(result.begin(), result.end());
    return PCSeries(std::move(result));
}

PCSeries PCRobMonitorVisitor::visitLookup(const LookupExpr *expr) {
    return callWithDelaySeries(_monitor, expr->delay(), expr->arg(), [&](Time delay, const PCSeries &argSeries) {
        return PCSeries::backshift(argSeries, delay, expr->defaultValue());    
    });
}

PCSeries PCRobMonitorVisitor::visitVar(const VarExpr *) {
    throw std::runtime_error("Should be unreachable. Variable expression should be evaluated by the Monitor directly.");
}

PCSeries PCRobMonitorVisitor::visitOn(const OnExpr *) {
    throw std::runtime_error("Only standard STL operators are supported.");    
}

PCSeries PCRobMonitorVisitor::visitAggregate(const AggregateExpr *) {
    throw std::runtime_error("Only standard STL operators are supported.");    
}

PCSeries PCRobMonitorVisitor::visitUntil(const UntilExpr *) {
    throw std::runtime_error("Only standard STL operators are supported.");    
}

PCSeries PCRobMonitorVisitor::visitGet(const GetExpr *) {
    throw std::runtime_error("Only standard STL operators are supported.");    
}

}
