#ifndef STLE_EXPR_SET_HPP
#define STLE_EXPR_SET_HPP

#include "Api.hpp"

#include "ExprAll.hpp"
#include "U/MemoryU.hpp"
#include "U/RefCountCache.hpp"

#include <unordered_set>
#include <memory>

namespace Stle {

// NOTE: Deriving from U::CannotCopy disables copy constructor and assignment to make sure an object is not copied accidentally.
// If ExprSet genuinely needs to be copied, remove this base class.
#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API ExprSet : U::CannotCopy {
public:
    ExprPtr makeConst(Space value);
    
    ExprPtr makeConst(int value);
    
    ExprPtr makeConst(double value);
    
    ExprPtr makeBoolConst(bool value);
    
    ExprPtr makeVar(const char *name);
    
    ExprPtr makeApply(ApplyOp op, const ExprPtr &arg);       
    
    /*!\brief Creates a function application.
      
       This verision will make redundant copies of the argument pointers,
       if possible, avoid using it in non-test code.
      
       \param op
       \param args
       \return 
     */
    // TODO: This actually is used in non-test code. Think how to avoid.
    // NOTE: The overload accepting an initializer list has to exist separately,
    // the forwarding overload won't work for initializer lists.
    ExprPtr makeApply(ApplyOp op, std::initializer_list<ExprPtr> args);
    
    template <typename... TArgs>
    ExprPtr makeApply(ApplyOp op, TArgs&&... args) {
        return make<ApplyExpr>(op, std::forward<TArgs>(args)...);
    }
    
    ExprPtr makeOn(const ClosedIntvl &intvl, const ExprPtr &arg);
    
    ExprPtr makeAggregate(AggregateOp op, const ExprPtr &arg);
    
    ExprPtr makeUntil(const ClosedIntvl &intvl, Space defaultValue, const ExprPtr &arg1, const ExprPtr &arg2);
    
    ExprPtr makeGet(const ExprPtr &arg);
    
    ExprPtr makeLookup(Time delay, Space defaultValue, const ExprPtr &arg);
    
    ExprPtr makeTemporal(const ClosedIntvl &intvl, TemporalOp op, const ExprPtr &arg);
    
    ExprPtr makeTemporalUntil(const ClosedIntvl &intvl, const ExprPtr &arg1, const ExprPtr &arg2);
    
private:
    template<typename TExpr, typename... TArgs>    
    ExprPtr make(TArgs&&... args) {
        std::shared_ptr<Expr> pExpr(_exprs.make<TExpr>(std::forward<TArgs>(args)...));
        // return ExprPtr(static_cast<TExpr*>(pExpr.get()));        
        return ExprPtr(std::move(pExpr));
    }
    
    U::RefCountCache<Expr, ExprHash> _exprs;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
