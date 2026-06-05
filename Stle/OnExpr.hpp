#ifndef STLE_ON_EXPR_HPP
#define STLE_ON_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ClosedIntvl.hpp"
#include "ExprError.hpp"
#include "ExprPtr.hpp"

namespace Stle {

class STLE_API OnExpr : public Expr {
public:
    OnExpr(const ClosedIntvl &intvl, const ExprPtr &arg)
        : Expr(EXPR_ON, EXPR_KIND_POINT), _intvl(intvl), _arg(arg) {
        if (arg->kind() != EXPR_KIND_INTVL)
            throw ExprError("Argument should be an interval expression.");
    }
    
    const ClosedIntvl &intvl() const {
        return _intvl;
    }
    
    const ExprPtr &arg() const {
        return _arg;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    ClosedIntvl _intvl;
    ExprPtr _arg;    
};

}

#endif
