#ifndef STLE_UNTIL_EXPR_HPP
#define STLE_UNTIL_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ClosedIntvl.hpp"
#include "ExprPtr.hpp"

namespace Stle {

class STLE_API UntilExpr : public Expr {
public:
    UntilExpr(const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2);
    
    const ClosedIntvl &intvl() const {
        return _intvl;
    }
    
    const Space defaultValue() const {
        return _defaultValue;
    }
    
    const ExprPtr &arg1() const {
        return _arg1;
    }
    
    const ExprPtr &arg2() const {
        return _arg2;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    ClosedIntvl _intvl;
    Space _defaultValue;
    ExprPtr _arg1;
    ExprPtr _arg2;
};

}

#endif
