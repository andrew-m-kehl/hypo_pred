#ifndef STLE_CMD_PC_STATE_HPP
#define STLE_CMD_PC_STATE_HPP

#include "Stle/Api.hpp"

#include "State.hpp"
#include "Result.hpp"
#include "SpaceResult.hpp"
#include "ErrorResult.hpp"
#include "OkResult.hpp"

#include "Stle/U/FileReader.hpp"
#include "Stle/U/CsvReader.hpp"

#include "Stle/PCSignal.hpp"
#include "Stle/PCMonitor.hpp"
#include "Stle/PCRobMonitor.hpp"
#include "Stle/ExprSet.hpp"
#include "Stle/CsvPCSignalReader.hpp"
#include "Stle/SExprParser.hpp"

namespace Stle {
namespace Cmd { 

class STLE_API PCState : public State {
public:
    PCState()
        : _vars(VARS_XN, 0), _monitor(_signal, _vars, _exprSet), _robMonitor(_signal, _vars, _exprSet), _readSignalFlags(READ_SIGNAL_DEFAULT)  {
    }
    
    void clearSignal() override;
    
    void readSignalFile(const char *name) override;
    
    void eval(const ExprPtr &expr) override;
    
    void evalFile(const char *name) override;
    
    void evalR(const ExprPtr &expr) override;
    
    void evalFileR(const char *name) override;
    
    void setInSignalFormat(ReadSignalFlags flags) override;
    
    void clearMonitor() override;   
    
private:
    
    void clearMonitorImpl();
    
    void evalImpl(const ExprPtr &expr);
    
    void evalImplR(const ExprPtr &expr);
    
    
    PCSignal _signal;
    SignalVars _vars;
    PCMonitor _monitor;
    PCRobMonitor _robMonitor;
    ReadSignalFlags _readSignalFlags;
    
};

}
}

#endif
