#ifndef STLE_AGGREGATE_EXPR_HPP
#define STLE_AGGREGATE_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ExprError.hpp"
#include "ExprPtr.hpp"

namespace Stle {

enum AggregateOp {
    AGGREGATE_MIN,
    AGGREGATE_MAX,
    AGGREGATE_INT
};

template <typename TPrinter>
void printAggregateOp(TPrinter &p, AggregateOp op) {
    switch (op) {
    case AGGREGATE_MIN:
        p << "Min";
        return;
        
    case AGGREGATE_MAX:
        p << "Max";
        return;

    case AGGREGATE_INT:
        p << "I";
        return;

    default:
        throw std::runtime_error("Unsupported aggregate operation.");
    }
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, AggregateOp op) {
    printAggregateOp(p, op);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, AggregateOp op);

class STLE_API AggregateExpr : public Expr {
public:
    AggregateExpr(AggregateOp op, const ExprPtr &arg)
        : Expr(EXPR_AGGREGATE, EXPR_KIND_INTVL), _arg(arg), _op(op)  {
        if (arg->kind() != EXPR_KIND_POINT)
            throw ExprError("Argument should be a point expression.");
    }
    
    AggregateOp op() const {
        return _op;
    }
    
    const ExprPtr &arg() const {
        return _arg;
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    ExprPtr _arg;
    AggregateOp _op;
};

}

#endif
