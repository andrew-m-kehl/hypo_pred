#ifndef STLE_LOOKUP_EXPR_HPP
#define STLE_LOOKUP_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ExprPtr.hpp"

namespace Stle {

class STLE_API LookupExpr : public Expr {
public:
    LookupExpr(Time delay, Space defaultValue, const ExprPtr &arg)
        : Expr(EXPR_LOOKUP, EXPR_KIND_POINT), _delay(delay), _defaultValue(defaultValue), _arg(arg) {        
    }
    
    Time delay() const {
        return _delay;
    }
    
    Space defaultValue() const {
        return _defaultValue;
    }
    
    const ExprPtr &arg() const {
        return _arg;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    Time _delay;
    Space _defaultValue;
    ExprPtr _arg;
};

}

#endif
