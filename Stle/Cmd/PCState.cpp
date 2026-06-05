#include "PCState.hpp"

namespace Stle {
namespace Cmd {

void PCState::clearSignal() {
    _signal = PCSignal();
    _vars = SignalVars(VARS_XN, 0);
    clearMonitorImpl();
    _result = std::make_unique<OkResult>();
}

void PCState::readSignalFile(const char *name) {
    try {
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, name, "r");
        _signal.add(readPCSignalCsv(csvReader, _readSignalFlags));
        // The below is not supposed to throw.
        _vars = SignalVars(VARS_XN, _signal.size());
        clearMonitorImpl();
        _result = std::make_unique<OkResult>();
        
    } catch (std::runtime_error &e) {
        // TODO: Making Result a union wil cause to not allocate on error path (dangerous in case of oom).
        _result = std::make_unique<ErrorResult>(e.what());
    }        
}

void PCState::eval(const ExprPtr &expr) {
    try {
        evalImpl(expr);
    } catch (std::runtime_error &e) {
        _result = std::make_unique<ErrorResult>(e.what());
    }  
}

void PCState::evalFile(const char *name) {        
    try {
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(_exprSet, U::OF_READER, U::OF_FILE, name, "r");
        ExprPtr expr = parser.parse();
        evalImpl(expr);
    } catch (std::runtime_error &e) {
        _result = std::make_unique<ErrorResult>(e.what());
    } 
}

void PCState::evalR(const ExprPtr &expr) {
    try {
        evalImplR(expr);
    } catch (std::runtime_error &e) {
        _result = std::make_unique<ErrorResult>(e.what());
    }  
}

void PCState::evalFileR(const char *name) {        
    try {
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(_exprSet, U::OF_READER, U::OF_FILE, name, "r");
        ExprPtr expr = parser.parse();
        evalImplR(expr);
    } catch (std::runtime_error &e) {
        _result = std::make_unique<ErrorResult>(e.what());
    } 
}

void PCState::setInSignalFormat(ReadSignalFlags flags) {
    _readSignalFlags = flags;
}

void PCState::clearMonitor() {
    clearMonitorImpl();
    _result = std::make_unique<OkResult>();
}

void PCState::clearMonitorImpl() {
    _monitor = PCMonitor(_signal, _vars, _exprSet);
    _robMonitor = PCRobMonitor(_signal, _vars, _exprSet);
}

void PCState::evalImpl(const ExprPtr &expr) {
    const PCSeries &outSeries = _monitor.makeOutput(expr);
    _result = std::make_unique<SpaceResult>(outSeries[0].value());
}

void PCState::evalImplR(const ExprPtr &expr) {
    const PCSeries &outSeries = _robMonitor.makeOutput(expr);
    _result = std::make_unique<SpaceResult>(outSeries[0].value());
}

}
}
