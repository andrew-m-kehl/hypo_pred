#ifndef STLE_CONST_EXPR_HPP
#define STLE_CONST_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"

namespace Stle {

class STLE_API ConstExpr : public Expr {
public:
    ConstExpr(Space value) : Expr(EXPR_CONST, EXPR_KIND_POINT), _value(value) {            
    }
    
    ConstExpr(int value) : ConstExpr(Space(value)) {
    }
    
    ConstExpr(double value) : ConstExpr(Space(value)) {
    }
    
    Space value() const {
        return _value;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    Space _value;
};

}

#endif
