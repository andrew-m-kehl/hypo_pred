#ifndef STLE_TEMPORAL_UNTIL_EXPR_HPP
#define STLE_TEMPORAL_UNTIL_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ClosedIntvl.hpp"
#include "ExprPtr.hpp"

namespace Stle {

class STLE_API TemporalUntilExpr : public Expr {
public:
    TemporalUntilExpr(const ClosedIntvl &intvl, const ExprPtr &arg1, const ExprPtr &arg2);
    
    const ClosedIntvl &intvl() const {
        return _intvl;
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
    ExprPtr _arg1;
    ExprPtr _arg2;
};

}

#endif
