#include "ExprAll.hpp"

#include "U/VOstreamRef.hpp"

namespace Stle {

ExprKind exprKindMeet(ExprKind k1, ExprKind k2) {
    static constexpr ExprKind meetTable[4][4] {
        {EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM},
        {EXPR_KIND_BOTTOM, EXPR_KIND_POINT,  EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM},
        {EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM, EXPR_KIND_INTVL,  EXPR_KIND_UNTIL},
        {EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM, EXPR_KIND_UNTIL,  EXPR_KIND_UNTIL}
    };
    return meetTable[static_cast<uint32_t>(k1)][static_cast<uint32_t>(k2)];
}


std::ostream &operator<<(std::ostream &os, const Expr &expr) {
    U::VOstreamRef osRef(os);
    expr.print(osRef);    
    return os;
}

std::ostream &operator<<(std::ostream &os, const Expr *expr) {
    return os << *expr;
}

}
