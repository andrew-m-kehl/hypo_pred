#ifndef STLE_CMD_STATE_HPP
#define STLE_CMD_STATE_HPP

#include "Stle/Api.hpp"

#include "Result.hpp"
#include "OkResult.hpp"

#include "Stle/ExprAll.hpp"
#include "Stle/ExprSet.hpp"
#include "Stle/ExprPtr.hpp"
#include "Stle/ReadSignalFlags.hpp"

namespace Stle {
namespace Cmd {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API State {
public:
    State() : _result(std::make_unique<OkResult>()) {        
    }
    
    virtual void clearSignal() = 0;
    virtual void readSignalFile(const char *name) = 0;
    virtual void eval(const ExprPtr &expr) = 0;
    virtual void evalFile(const char *name) = 0;
    virtual void evalR(const ExprPtr &expr) = 0;
    virtual void evalFileR(const char *name) = 0;
    virtual void setInSignalFormat(ReadSignalFlags flags) = 0;
    virtual void clearMonitor() = 0;
    
    void setOkResult();
    void setErrorResult(const char *msg);
    void setStringResult(const char *str);
    
    
    virtual ~State() = default;
    // Need to re-enable moving after adding a virtual destructor.    
    State(State &&) = default;
    State &operator=(State &&) = default;
    State(const State &) = default;
    State &operator=(const State &) = default;
    
    ExprSet &exprSet() {
        return _exprSet;
    }
    
    const Result &result() const {
        return *_result;
    }
    
protected:
    ExprSet _exprSet;
    std::unique_ptr<Result> _result;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}
}

#endif
