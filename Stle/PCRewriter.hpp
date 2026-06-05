#ifndef STLE_OFFLINE_PC_REWRITER_HPP
#define STLE_OFFLINE_PC_REWRITER_HPP

#include "Api.hpp"

#include "ExprAll.hpp"
#include "ExprSet.hpp"
#include "ExprPtrVisitor.hpp"

namespace Stle {

STLE_API ExprPtr rewriteForPCMonitor(ExprSet &exprSet, const ExprPtr &expr);

class STLE_API PCRewriter : public ExprPtrVisitor<PCRewriter, ExprPtr> {
public:
    PCRewriter(ExprSet &exprSet) : _exprSet(&exprSet) {
    }

    ExprPtr visitConst(const ExprPtr&, const ConstExpr*);
    
    ExprPtr visitBoolConst(const ExprPtr&, const BoolConstExpr*);

    ExprPtr visitVar(const ExprPtr&, const VarExpr*);

    ExprPtr visitApply(const ExprPtr&, const ApplyExpr*);

    ExprPtr visitOn(const ExprPtr&, const OnExpr*);

    ExprPtr visitAggregate(const ExprPtr&, const AggregateExpr*);

    ExprPtr visitUntil(const ExprPtr&, const UntilExpr*);

    ExprPtr visitGet(const ExprPtr&, const GetExpr*);

    ExprPtr visitLookup(const ExprPtr&, const LookupExpr*);

    ExprPtr visitTemporal(const ExprPtr&, const TemporalExpr*);

    ExprPtr visitTemporalUntil(const ExprPtr&, const TemporalUntilExpr*);
    
private:
    ExprSet *_exprSet;
};

}

#endif
