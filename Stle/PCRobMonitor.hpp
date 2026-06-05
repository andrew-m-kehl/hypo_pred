#ifndef STLE_PC_ROB_MONITOR_HPP
#define STLE_PC_ROB_MONITOR_HPP

#include "Api.hpp"

#include "PCMonitorBase.hpp"

namespace Stle {

class STLE_API PCRobMonitor : public PCMonitorBase<PCRobMonitor> {
public:
    PCRobMonitor(const PCSignal &input, const SignalVars &vars, ExprSet &exprSet)
        : PCMonitorBase<PCRobMonitor>(input, vars, exprSet) {
    }
    
private:
    PCSeries evalOutput(const ExprPtr &expr);
    
    friend class PCMonitorBase<PCRobMonitor>;
};

}

#endif
