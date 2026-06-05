#ifndef STLE_APPLY_EXPR_HPP
#define STLE_APPLY_EXPR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "ApplyOp.hpp"
#include "ExprError.hpp"
#include "ExprPtr.hpp"

namespace Stle {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API ApplyExpr : public Expr {
public:
    template<typename TIt>
    ApplyExpr(ApplyOp op, TIt begin, TIt end)
        : Expr(EXPR_APPLY, EXPR_KIND_BOTTOM), _args(begin, end), _op(op) {
        _kind = applyKind(_args);
        if (_kind == EXPR_KIND_BOTTOM)            
            throw ExprError("Arguments should be of unifiable kinds.");
        checkArity(op, _args);
    }
    
    ApplyExpr(ApplyOp op, const ExprPtr &arg);
    
    /*!\brief Constructor.
       
       This verision will make redundant copies of the argument pointers,
       if possible, avoid using it in non-test code.
      
       \param op Applied function.
       \param args Arguments of the function.
     */       
    ApplyExpr(ApplyOp op, std::initializer_list<ExprPtr> args);
    
    ApplyOp op() const {
        return _op;
    }
    
    int size() const {
        return static_cast<int>(_args.size());
    }
    
    const ExprPtr &at(int i) const {
#ifndef NDEBUG
        if (i < 0 || i >= size())
            throw std::runtime_error("Argument index out of range.");
#endif
        return _args[static_cast<size_t>(i)];
    }
    
    const ExprPtr &operator[](int i) const {
        return at(i);
    }
    
    void hash(U::Hasher &hasher) const override;
    
    bool equals(const Expr &other) const override;
    
    void print(U::VPrinter &os) const override;
    
private:
    static ExprKind applyKind(const std::vector<ExprPtr> &args);    
    
    static void checkArity(ApplyOp op, const std::vector<ExprPtr> &args);    
    
    std::vector<ExprPtr> _args;
    ApplyOp _op;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
