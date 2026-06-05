#ifndef STLE_EXPR_PTR_HPP
#define STLE_EXPR_PTR_HPP

#include "Api.hpp"

#include "Expr.hpp"
#include "U/Hasher.hpp"

#include <memory>

namespace Stle {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API ExprPtr {
public:
    ExprPtr() {        
    }
    
    explicit ExprPtr(std::shared_ptr<Expr> &&expr)
        : _expr(std::move(expr)) {
    }
    
    const Expr &operator*() const {
        return *_expr;
    }
    
    const Expr *operator->() const {
        return _expr.get();
    }
    
    operator bool() const {
        return _expr.get();
    }
    
    const Expr *get() const {
        return _expr.get();
    }
    
    bool operator==(const ExprPtr &other) const {
        return _expr == other._expr;
    }
    
    bool operator!=(const ExprPtr &other) const {
        return !(*this == other);
    }
    
    void hash(U::Hasher &hasher) const {
        hasher.add(_expr.get());
    }
    
private:
    std::shared_ptr<Expr> _expr;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

struct STLE_API ExprPtrHash {
    size_t operator()(const ExprPtr &expr) const {
        U::Hasher hasher;
        expr.hash(hasher);
        return hasher.hash();
    }
};

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const ExprPtr &expr) {
    return p << *expr.get();
}

STLE_API std::ostream &operator<<(std::ostream &os, const ExprPtr &expr);

}

#endif
