#ifndef STLE_EXPR_VISITOR_HPP
#define STLE_EXPR_VISITOR_HPP

#include "ExprAll.hpp"

namespace Stle {

template <typename TVisitor, typename TResult>
class ExprVisitor {
public:
    using result_type = TResult;

    template <typename... TArgs>
    result_type visit(const Expr *expr, TArgs&&... args) {
        switch (expr->type()) {
        case EXPR_CONST:
            return static_cast<TVisitor*>(this)->visitConst(static_cast<const ConstExpr*>(expr), std::forward<TArgs>(args)...);
            
        case EXPR_BOOL_CONST:
            return static_cast<TVisitor*>(this)->visitBoolConst(static_cast<const BoolConstExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_VAR:
            return static_cast<TVisitor*>(this)->visitVar(static_cast<const VarExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_APPLY:
            return static_cast<TVisitor*>(this)->visitApply(static_cast<const ApplyExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_ON:
            return static_cast<TVisitor*>(this)->visitOn(static_cast<const OnExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_AGGREGATE:
            return static_cast<TVisitor*>(this)->visitAggregate(static_cast<const AggregateExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_UNTIL:
            return static_cast<TVisitor*>(this)->visitUntil(static_cast<const UntilExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_GET:
            return static_cast<TVisitor*>(this)->visitGet(static_cast<const GetExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_LOOKUP:
            return static_cast<TVisitor*>(this)->visitLookup(static_cast<const LookupExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_TEMPORAL:
            return static_cast<TVisitor*>(this)->visitTemporal(static_cast<const TemporalExpr*>(expr), std::forward<TArgs>(args)...);

        case EXPR_TEMPORAL_UNTIL:
            return static_cast<TVisitor*>(this)->visitTemporalUntil(static_cast<const TemporalUntilExpr*>(expr), std::forward<TArgs>(args)...);

        default:
            throw std::runtime_error("Unknown expression type.");
        }
    }
};

}

#endif
