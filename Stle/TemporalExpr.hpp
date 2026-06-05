#ifndef STLE_TEMPORAL_EXPR_HPP
#define STLE_TEMPORAL_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ClosedIntvl.hpp"
#include "ExprError.hpp"
#include "ExprPtr.hpp"

namespace Stle {

enum TemporalOp {
    TEMPORAL_F,
    TEMPORAL_G
};

template <typename TPrinter>
void printTemporalOp(TPrinter &p, TemporalOp op) {
    switch (op) {
    case TEMPORAL_F:
        p << "F";
        return;
        
    case TEMPORAL_G:
        p << "G";
        return;
        
    default:
        throw std::runtime_error("Unsupported temporal operation.");
    }
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, TemporalOp op) {
    printTemporalOp(p, op);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, TemporalOp op);

class STLE_API TemporalExpr : public Expr {
public:
    TemporalExpr(const ClosedIntvl intvl, TemporalOp op, const ExprPtr &arg)
        : Expr(EXPR_TEMPORAL, EXPR_KIND_POINT), _intvl(intvl), _arg(arg), _op(op) {
        if (arg->kind() != EXPR_KIND_POINT)
            throw ExprError("Argument should be a point expression.");
    }
    
    const ClosedIntvl &intvl() const {
        return _intvl;
    }
    
    TemporalOp op() const {
        return _op;
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
    TemporalOp _op;
};

}

#endif
