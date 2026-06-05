#ifndef STLE_GET_EXPR_HPP
#define STLE_GET_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ExprError.hpp"
#include "ExprPtr.hpp"

namespace Stle {

class STLE_API GetExpr : public Expr {
public:
    GetExpr(const ExprPtr &arg) : Expr(EXPR_GET, EXPR_KIND_UNTIL), _arg(arg) {
        if (arg->kind() != EXPR_KIND_POINT)
            throw ExprError("Argument should be a point expression.");
    }
    
    const ExprPtr &arg() const {
        return _arg;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    ExprPtr _arg;
};

}

#endif
