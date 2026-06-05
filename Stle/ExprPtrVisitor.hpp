#ifndef STLE_EXPR_PTR_VISITOR_HPP
#define STLE_EXPR_PTR_VISITOR_HPP

#include "Api.hpp"

#include "ExprVisitor.hpp"
#include "Expr.hpp"
#include "ExprPtr.hpp"

#include <utility>

namespace Stle {

template<typename TVisitor, typename TResult>
class ExprPtrVisitor {
public:
    using result_type = TResult;    
    
    template <typename... TArgs>
    result_type visit(const ExprPtr &expr, TArgs&&... args) {
        ExprPtrExprVisitor visitor;
        return visitor.visit(expr.get(), expr, static_cast<TVisitor&>(*this), std::forward<TArgs>(args)...);
    }
    
private:
    class ExprPtrExprVisitor : public ExprVisitor<ExprPtrExprVisitor, result_type> {
    public:
        template<typename... TArgs>
        result_type visitConst(const ConstExpr* constExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitConst(expr, constExpr, std::forward<TArgs>(args)...);
        }
        
        template<typename... TArgs>
        result_type visitBoolConst(const BoolConstExpr* constExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitBoolConst(expr, constExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>    
        result_type visitVar(const VarExpr *varExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitVar(expr, varExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitApply(const ApplyExpr *applyExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitApply(expr, applyExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitOn(const OnExpr *onExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitOn(expr, onExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitAggregate(const AggregateExpr *aggregateExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitAggregate(expr, aggregateExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitUntil(const UntilExpr *untilExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitUntil(expr, untilExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitGet(const GetExpr *getExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitGet(expr, getExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitLookup(const LookupExpr *lookupExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitLookup(expr, lookupExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitTemporal(const TemporalExpr *temporalExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
             return visitor.visitTemporal(expr, temporalExpr, std::forward<TArgs>(args)...);
        }
    
        template<typename... TArgs>
        result_type visitTemporalUntil(const TemporalUntilExpr *temporalUntilExpr, const ExprPtr &expr, TVisitor &visitor, TArgs&&... args) {
            return visitor.visitTemporalUntil(expr, temporalUntilExpr, std::forward<TArgs>(args)...);
        }
    };
};

}

#endif
