#ifndef STLE_CMD_CMD_RUNNER_HPP
#define STLE_CMD_CMD_RUNNER_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"
#include "SCmdParser.hpp"
#include "CmdRunnerVisitor.hpp"

#include "Stle/U/VPrinter.hpp"
#include "Stle/U/ReaderRef.hpp"
#include "Stle/U/OfPrinter.hpp"

namespace Stle {
namespace Cmd {

template <typename TState, typename TRunVisitor, typename TPrinter>
class CmdRunner {
public:
    template<typename... TArgs>
    CmdRunner(U::OfPrinterTag, TArgs&&... args) : _printer(std::forward<TArgs>(args)...), _vprinter(_printer) {
    }
    
    void run(const Cmd &cmd);
    
    template<typename TReader, typename... TArgs>
    void runReader(TArgs&&... args);
    
    void runStr(const char *str);
    
    const Result &result() const {
        return _state.result();
    }
    
    const TPrinter &printer() const {
        return _printer;
    }
    
private:
    TState _state;
    TPrinter _printer;
    U::VPrinterRef<TPrinter> _vprinter;
};

template <typename TState, typename TRunVisitor, typename TPrinter>
void CmdRunner<TState,TRunVisitor,TPrinter>::run(const Cmd &cmd) {
    visitCmd<TRunVisitor>(cmd, _state);
    _state.result().print(_vprinter);
    _vprinter << '\n';
    _vprinter.flush();
}

template <typename TState, typename TRunVisitor, typename TPrinter>
template<typename TReader, typename... TArgs>
void CmdRunner<TState,TRunVisitor,TPrinter>::runReader(TArgs&&... args) {
    SCmdParser<U::SLexer<TReader>> parser(_state.exprSet(), std::forward<TArgs>(args)...);
    while (true) {
        std::unique_ptr<Cmd> cmd = parser.parse();
        if (cmd->type() == CMD_EOF)
            break;
        run(*cmd);
    }
}

template <typename TState, typename TRunVisitor, typename TPrinter>
void CmdRunner<TState,TRunVisitor,TPrinter>::runStr(const char *str) {
    runReader<U::StrReader>(U::OF_READER, str);
}

}
}

#endif
