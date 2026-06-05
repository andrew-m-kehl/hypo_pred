#ifndef STLE_OFFLINE_PC_MONITOR_HPP
#define STLE_OFFLINE_PC_MONITOR_HPP

#include "Api.hpp"

#include "PCMonitorBase.hpp"

namespace Stle {

class STLE_API PCMonitor : public PCMonitorBase<PCMonitor> {
public:
    PCMonitor(const PCSignal &input, const SignalVars &vars, ExprSet &exprSet)
        : PCMonitorBase<PCMonitor>(input, vars, exprSet) {
    }
    
private:
    PCSeries evalOutput(const ExprPtr &expr);
    
    friend class PCMonitorBase<PCMonitor>;    
};

}

#endif
