#ifndef STLE_CMD_RESULT_HPP
#define STLE_CMD_RESULT_HPP

#include "Stle/Api.hpp"

#include "ResultType.hpp"
#include "Stle/U/VPrinter.hpp"
#include "Stle/U/VPrinterRef.hpp"

namespace Stle {
namespace Cmd {

// TODO: Perhaps make this into a union or variant.
class STLE_API Result {
public:
    explicit Result(ResultType type) : _type(type) {        
    }
    
    ResultType type() const {
        return _type;
    }
    
    virtual bool equals(const Result &other) const = 0;
    
    virtual void print(U::VPrinter &p) const = 0;
    
    bool operator==(const Result &other) const {
        return equals(other);
    }
    
    bool operator!=(const Result &other) const {
        return !(*this == other);
    }
    
    virtual ~Result() = default;
    // Need to re-enable moving after adding a virtual destructor.
    Result(Result &&) = default;
    Result &operator=(Result &&) = default;
    Result(const Result &) = default;
    Result &operator=(const Result &) = default;
    
protected:
    ResultType _type;
};

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const Result &expr) {
    U::VPrinterRef<TPrinter> pRef(p);
    expr.print(pRef);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, const Result &expr);

}
}

#endif
