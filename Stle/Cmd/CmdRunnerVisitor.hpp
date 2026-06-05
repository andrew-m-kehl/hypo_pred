#ifndef STLE_CMD_CMD_RUNNER_VISITOR_HPP
#define STLE_CMD_CMD_RUNNER_VISITOR_HPP

#include "Stle/Api.hpp"

#include "CmdVisitor.hpp"
#include "State.hpp"

#include "Stle/U/Unit.hpp"

namespace Stle {
namespace Cmd {

class STLE_API RunCmdVisitor : public CmdVisitor<RunCmdVisitor, void> {
public:
    RunCmdVisitor(State &state) : _state(&state) {        
    }

    void visitReadSignalCsv(const ReadSignalCsvCmd&);

    void visitEval(const EvalCmd&);

    void visitEvalFile(const EvalFileCmd&);
    
    void visitEvalR(const EvalRCmd&);

    void visitEvalFileR(const EvalFileRCmd&);

    void visitClearMonitor(const ClearMonitorCmd&);

    void visitVersion(const VersionCmd&);
    
    void visitSetReadSignalFlags(const SetReadSignalFlagsCmd&);

    void visitEof(const EofCmd&);
    
protected:
    State *_state;    
};
            
}
}



#endif
