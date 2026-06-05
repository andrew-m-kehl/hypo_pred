#ifndef STLE_VAR_EXPR_HPP
#define STLE_VAR_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"

namespace Stle {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API VarExpr : public Expr {
public:
    VarExpr(const char *name) : Expr(EXPR_VAR, EXPR_KIND_POINT), _name(name) {            
    }
    
    VarExpr(const std::string &name) : Expr(EXPR_VAR, EXPR_KIND_POINT), _name(name) {            
    }
    
    const std::string name() const {
        return _name;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    std::string _name;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
