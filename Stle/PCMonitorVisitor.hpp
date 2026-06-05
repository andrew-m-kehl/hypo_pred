#ifndef STLE_OFFLINE_PC_MONITOR_VISITOR_HPP
#define STLE_OFFLINE_PC_MONITOR_VISITOR_HPP

#include "Api.hpp"

#include "ExprVisitor.hpp"
#include "PCSeries.hpp"
#include "IntvlEnd.hpp"
#include "PCMonitor.hpp"
#include "PCMonitorVisitorU.hpp"

namespace Stle {

class STLE_API PCMonitorVisitor : public ExprVisitor<PCMonitorVisitor, PCSeries> {
public:
    PCMonitorVisitor(PCMonitor &monitor)
        : _monitor(&monitor) {        
    }

    PCSeries visitConst(const ConstExpr*);
    
    PCSeries visitBoolConst(const BoolConstExpr*);

    PCSeries visitVar(const VarExpr*);

    PCSeries visitApply(const ApplyExpr*);

    PCSeries visitOn(const OnExpr*);

    PCSeries visitAggregate(const AggregateExpr*);

    PCSeries visitUntil(const UntilExpr*);

    PCSeries visitGet(const GetExpr*);

    PCSeries visitLookup(const LookupExpr*);

    PCSeries visitTemporal(const TemporalExpr*);

    PCSeries visitTemporalUntil(const TemporalUntilExpr*);
    
    template <typename TInit, typename TOp>
    static PCSeries untimedUntil(const PCSeries &series1, const PCSeries &series2, TInit init, TOp op, Space defaultValue);    
    
    template <typename TOp>
    static PCSeries untimedUntil(const PCSeries &series1, const PCSeries &series2, TOp op, Space defaultValue) {
        return untimedUntil(series1, series2, SpaceId{}, op, defaultValue);
    }
    
 private:    
    enum UntilState {
        UNTIL_OUT1,
        UNTIL_OUT_AGG1,
        UNTIL_OUT_DEFAULT
    };
    
    PCMonitor *_monitor;    
};

// TODO: Need more tests for until.
template<typename TInit, typename TOp>
PCSeries PCMonitorVisitor::untimedUntil(const PCSeries &series1, const PCSeries &series2, TInit init, TOp op, Space defaultValue) {    
#ifndef NDEBUG
    if (series1.empty() || series2.empty())
        throw std::runtime_error("Series is empty.");
#endif
    
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
    Time seriesEnd = nextEnd1.end();
    
    std::vector<PCSeg> result;
    result.reserve(static_cast<size_t>(std::max(series1.size(), series2.size())));

    UntilState state = UNTIL_OUT_DEFAULT;
    // Will be updated in the first iteration.
    Space value1(0);
    Space outputValue = defaultValue;    
    
    while(i1 >= 0 || i2 >= 0) {        
        bool update1 = false;
        bool update2 = false;
        Time segmentEnd(0);
        if (nextEnd1 > nextEnd2) {
            update1 = true;
            segmentEnd = nextEnd1.end();
        }
        else if (nextEnd1 < nextEnd2) {
            update2 = true;
            segmentEnd = nextEnd2.end();
        }
        else { // if (end1 == end2)
            update1 = true;
            update2 = true;
            segmentEnd = nextEnd1.end();
        }
        
        bool changed2 = false;
        if (update2) {
            if (nextValue2 != Space(0) && state != UNTIL_OUT1) {
                state = UNTIL_OUT1;
                changed2 = true;
            }
            else if (nextValue2 == Space(0) && state == UNTIL_OUT1) {
                state = UNTIL_OUT_AGG1;
                changed2 = true;
            }
            --i2;
            untilRead(series2, i2, nextEnd2, nextValue2);            
        }
        
        bool changed1 = false;
        if (update1) {
            value1 = nextValue1;
            
            switch (state) {
            case UNTIL_OUT1:
                outputValue = init(value1);                    
                changed1 = true;
                break;
                
            case UNTIL_OUT_AGG1:                
                outputValue = op(outputValue, init(value1));
                changed1 = true;
                break;
                
            // case UNTIL_OUT_DEFAULT:
            default:
                // While outputing default value, do not care about changes in arg1, 
                // but need to output the default value once or twice in the end of the signal.
                changed1 = (segmentEnd == seriesEnd);
                break;                
            }            
            --i1;
            untilRead(series1, i1, nextEnd1, nextValue1);
        }
        else if (changed2 && state == UNTIL_OUT1) {
            outputValue = init(value1);
        }
        
        if (changed1 || changed2)
            untilAdd(result, segmentEnd, outputValue);
    }
    
    std::reverse(result.begin(), result.end());
    return PCSeries(std::move(result));        
}

}

#endif
