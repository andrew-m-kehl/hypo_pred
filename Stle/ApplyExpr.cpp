#include "ApplyExpr.hpp"
#include "ExprError.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, ApplyOp op) {
    printApplyOp(os, op);
    return os;
}

ApplyExpr::ApplyExpr(ApplyOp op, const ExprPtr &arg)
    : Expr(EXPR_APPLY, EXPR_KIND_BOTTOM), _args(1, arg), _op(op) {
    _kind = applyKind(_args);
    if (_kind == EXPR_KIND_BOTTOM)            
        throw ExprError("Arguments should be of unifiable kinds.");
    checkArity(op, _args);
}

ApplyExpr::ApplyExpr(ApplyOp op, std::initializer_list<ExprPtr> args)
    : ApplyExpr(op, args.begin(), args.end()) {    
}

ExprKind ApplyExpr::applyKind(const std::vector<ExprPtr> &args) {
    auto begin = args.begin();
    auto end = args.end();
    if (begin == end)
        return EXPR_KIND_POINT;
    ExprKind result = (*begin)->kind();    
    for (auto it = std::next(begin); it != end && result != EXPR_KIND_BOTTOM; ++it)
        result = exprKindMeet(result, (*it)->kind());
    return result;
}

void ApplyExpr::checkArity(ApplyOp op, const std::vector<ExprPtr> &args) {
    auto count = std::distance(args.begin(), args.end());
    switch (op) {
    case APPLY_MINUS:
        if (count == 0)
            throw ExprError("Minus should have at least one argument.");
        break;
        
    case APPLY_DIV:
        if (count != 2)
            throw ExprError("Division should have two arguments.");
        break;
        
    case APPLY_ABS:    
        if (count != 1)
            throw ExprError("Abs should have one argument.");
        break;
        
    case APPLY_BOOL_NOT:
        if (count != 1)
            throw ExprError("Boolean Not should have one argument.");
        break;
        
    case APPLY_BOOL_IMPL:
        if (count < 2)
            throw ExprError("Boolean Implication should have two or more arguments.");
        break;
        
    case APPLY_ITE:
        if (count != 3)
            throw ExprError("Ite should have three arguments.");
        break;
        
    case APPLY_LT:
    case APPLY_LEQ:
    case APPLY_GT:
    case APPLY_GEQ:
        if (count < 2)
            throw ExprError("Comparison should have two or more arguments.");
        break;
        
    default:
        break;
    }
}

void ApplyExpr::hash(U::Hasher &hasher) const {
    hasher.add(_type);
    hasher.add(_op);    
    hasher.add(_args);
}

bool ApplyExpr::equals(const Expr &other) const {
    if (_type != other.type())
        return false;
    
    const ApplyExpr &otherApplyExpr = static_cast<const ApplyExpr&>(other);
    return _op == otherApplyExpr._op && _args == otherApplyExpr._args;            
}

void ApplyExpr::print(U::VPrinter &p) const {
    p << "(";
    p << _op;
    for (int i = 0, count = size(); i < count; ++i) {
        p << " ";
        p << *at(i);
    }
    p << ")";            
}

}
