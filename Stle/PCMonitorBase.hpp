#ifndef STLE_OFFLINE_PC_MONITOR_BASE_HPP
#define STLE_OFFLINE_PC_MONITOR_BASE_HPP

#include "Api.hpp"

#include "ExprAll.hpp"
#include "PCSignal.hpp"
#include "SignalVars.hpp"
#include "ExprSet.hpp"
#include "PCRewriter.hpp"

#include <unordered_map>
#include <memory>

namespace Stle {

// NOTE: Deriving from U::CannotCopy disables copy constructor and assignment to make sure an object is not copied accidentally.
// If OfflinePCMonitors genuinely need to be copied, remove this base class.
template <typename TMonitor>
class PCMonitorBase : U::CannotCopy {
public:
    PCMonitorBase(const PCSignal &input, const SignalVars &vars, ExprSet &exprSet)
        : _input(&input), _vars(&vars), _exprSet(&exprSet) {
    }
    
    const PCSignal &input() const {
        return *_input;
    }
    
    const SignalVars &vars() const {
        return *_vars;
    }
    
    const PCSeries &makeOutput(const ExprPtr &expr, bool rewriteExpr = true, ExprPtr *rewrittenExpr = nullptr);
    
    const PCSeries &makeOutputUnchecked(const ExprPtr &expr);
    
    const PCSeries &makeReverse(const VarExpr *varExpr);
    
    void clear() {
        _series.clear();
    }
        
private:
    // Unordered_map does not invalidate references on insert, so, we can store PCSeries objects in
    // it directly.
    // NOTE: Monitor keeps the evaluated expressions alive.
    std::unordered_map<ExprPtr, PCSeries, ExprPtrHash> _series;
    
    // Stores reversed input signals.
    std::unordered_map<int, PCSeries> _reverseInputs;
    
    const PCSignal *_input;
    const SignalVars *_vars;
    ExprSet *_exprSet;    
};

template <typename TMonitor>
const PCSeries &PCMonitorBase<TMonitor>::makeOutput(const ExprPtr &expr, bool rewriteExpr, ExprPtr *rewrittenExpr) {
    // TODO: Perhaps another entry point that does not check things.
    if (rewriteExpr) {
        if (rewrittenExpr) {
            *rewrittenExpr = rewriteForPCMonitor(*_exprSet, expr);
            return makeOutput(*rewrittenExpr, false);
        }
        else {
            ExprPtr rewrittenExpr1(rewriteForPCMonitor(*_exprSet, expr)); 
            return makeOutput(rewrittenExpr1, false);
        }   
    }
    
    if (_input->empty())
        throw std::runtime_error("Input signal should not be empty.");
    
    return makeOutputUnchecked(expr);
}

// TODO: Maybe a shorter name for this.
template <typename TMonitor>
const PCSeries &PCMonitorBase<TMonitor>::makeOutputUnchecked(const ExprPtr &expr) {
    if (expr->type() == EXPR_VAR) {
        // Output series of a VarExpr refers to a series in the input signal and
        // does not have to be stored in _series.
        const VarExpr *varExpr = static_cast<const VarExpr*>(expr.get());            
        return _input->at(_vars->index(varExpr->name()));
    }
    
    PCSeries &result = _series[expr];
    // Valid output cannot be empty, because valid input cannot be empty.
    // If 'result' is empty, it was just created by the operator[] and needs to be evaluated.
    if (result.empty())
        result = static_cast<TMonitor*>(this)->evalOutput(expr);
    
    return result;
}

template <typename TMonitor>
const PCSeries &PCMonitorBase<TMonitor>::makeReverse(const VarExpr *varExpr) {
    int index = _vars->index(varExpr->name());
    PCSeries &result = _reverseInputs[index];
    if (result.empty())
        result = PCSeries::reverse(_input->at(index));
    return result;
}

}

#endif
