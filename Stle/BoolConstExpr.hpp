#ifndef STLE_BOOL_CONST_EXPR_HPP
#define STLE_BOOL_CONST_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"

namespace Stle {

class STLE_API BoolConstExpr : public Expr {
public:
    explicit BoolConstExpr(bool value) : Expr(EXPR_BOOL_CONST, EXPR_KIND_POINT), _value(value) {
    }
    
    bool value() const {
        return _value;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    bool _value;
};

}

#endif
