#ifndef STLE_CMD_CMD_VISITOR
#define STLE_CMD_CMD_VISITOR

#include "CmdAll.hpp"

namespace Stle {
namespace Cmd {

template <typename TVisitor, typename TResult1>
class CmdVisitor {
public:
    using TResult = TResult1;

    TResult visit(const Cmd &cmd) {
        switch (cmd.type()) {
        case CMD_READ_SIGNAL_CSV:
            return static_cast<TVisitor*>(this)->visitReadSignalCsv(static_cast<const ReadSignalCsvCmd&>(cmd));

        case CMD_EVAL:
            return static_cast<TVisitor*>(this)->visitEval(static_cast<const EvalCmd&>(cmd));

        case CMD_EVAL_FILE:
            return static_cast<TVisitor*>(this)->visitEvalFile(static_cast<const EvalFileCmd&>(cmd));
            
        case CMD_EVAL_R:
            return static_cast<TVisitor*>(this)->visitEvalR(static_cast<const EvalRCmd&>(cmd));

        case CMD_EVAL_FILE_R:
            return static_cast<TVisitor*>(this)->visitEvalFileR(static_cast<const EvalFileRCmd&>(cmd));

        case CMD_CLEAR_MONITOR:
            return static_cast<TVisitor*>(this)->visitClearMonitor(static_cast<const ClearMonitorCmd&>(cmd));

        case CMD_VERSION:
            return static_cast<TVisitor*>(this)->visitVersion(static_cast<const VersionCmd&>(cmd));
            
        case CMD_SET_READ_SIGNAL_FLAGS:
            return static_cast<TVisitor*>(this)->visitSetReadSignalFlags(static_cast<const SetReadSignalFlagsCmd&>(cmd));

        case CMD_EOF:
            return static_cast<TVisitor*>(this)->visitEof(static_cast<const EofCmd&>(cmd));

        default:
            throw std::runtime_error("Unknown comman type.");
        }
    }
};

template<typename TVisitor, typename... TArgs>
typename TVisitor::TResult visitCmd(const Cmd &cmd, TArgs&&... args) {
    TVisitor visitor(std::forward<TArgs>(args)...);
    return visitor.visit(cmd);
}
    
}
} 

#endif
