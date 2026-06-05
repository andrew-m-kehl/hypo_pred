#include "CmdRunnerVisitor.hpp"

#include "ReadSignalCsvCmd.hpp"
#include "EvalCmd.hpp"
#include "EvalFileCmd.hpp"

namespace Stle {
namespace Cmd {

void RunCmdVisitor::visitReadSignalCsv(const ReadSignalCsvCmd &cmd) {
    _state->readSignalFile(cmd.fileName().c_str());
}

void RunCmdVisitor::visitEval(const EvalCmd &cmd) {
    _state->eval(cmd.expr());
}

void RunCmdVisitor::visitEvalFile(const EvalFileCmd &cmd) {
    _state->evalFile(cmd.fileName().c_str());
}

void RunCmdVisitor::visitEvalR(const EvalRCmd &cmd) {
    _state->evalR(cmd.expr());
}

void RunCmdVisitor::visitEvalFileR(const EvalFileRCmd &cmd) {
    _state->evalFileR(cmd.fileName().c_str());
}

void RunCmdVisitor::visitClearMonitor(const ClearMonitorCmd &) {
    _state->clearMonitor();
}

void RunCmdVisitor::visitVersion(const VersionCmd &) {
    _state->setStringResult("unknown");
}

void RunCmdVisitor::visitSetReadSignalFlags(const SetReadSignalFlagsCmd &cmd) {
    _state->setInSignalFormat(cmd.flags());
}

void RunCmdVisitor::visitEof(const EofCmd &) {
}

} 
}
