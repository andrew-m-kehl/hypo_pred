#ifndef PC_ROB_MONITOR_VISITOR_HPP
#define PC_ROB_MONITOR_VISITOR_HPP

#include "Api.hpp"

#include "ExprVisitor.hpp"
#include "PCRobMonitor.hpp"

namespace Stle {

class STLE_API PCRobMonitorVisitor : public ExprVisitor<PCRobMonitorVisitor, PCSeries> {
public:
    PCRobMonitorVisitor(PCRobMonitor &monitor)
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
    
private:
    PCSeries untimedUntil(const PCSeries &series1, const PCSeries &series2);
    
    PCRobMonitor *_monitor;
};


}

#endif
