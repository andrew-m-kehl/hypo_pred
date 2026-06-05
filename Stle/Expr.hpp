#ifndef STLE_EXPR_FORWARD_HPP
#define STLE_EXPR_FORWARD_HPP

#include "Api.hpp"

#include "SpaceTime.hpp"

#include "U/HashU.hpp"
#include "U/VPrinter.hpp"
#include "U/VPrinterRef.hpp"

#include <vector>
#include <ostream>

namespace Stle { 

enum ExprType {
    EXPR_CONST,
    EXPR_BOOL_CONST,
    EXPR_VAR,    
    EXPR_APPLY,
    EXPR_ON,
    EXPR_AGGREGATE,
    EXPR_GET,
    EXPR_UNTIL,     
    EXPR_LOOKUP,
    EXPR_TEMPORAL,
    EXPR_TEMPORAL_UNTIL
};

enum ExprKind {
    EXPR_KIND_BOTTOM = 0,
    EXPR_KIND_POINT = 1,
    EXPR_KIND_INTVL = 2,
    EXPR_KIND_UNTIL = 3        
};

STLE_API ExprKind exprKindMeet(ExprKind k1, ExprKind k2);

// TODO: Perhaps Expr should point to the containing ExprSet.
class STLE_API Expr {        
public:
    Expr(ExprType type, ExprKind kind) : _type(type), _kind(kind) {
    }
    
    ExprType type() const {
        return _type;
    }
    
    ExprKind kind() const {
        return _kind;
    }
    
    virtual void hash(U::Hasher &hasher) const = 0;

    virtual bool equals(const Expr &other) const = 0;
    
    bool operator==(const Expr &other) const {
        return equals(other);
    }
    
    bool operator!=(const Expr &other) const {
        return !(*this == other);
    }
    
    virtual void print(U::VPrinter &p) const = 0;
    
    virtual ~Expr() = default;
    // Need to re-enable moving after adding a virtual destructor.
    Expr(Expr &&) = default;
    Expr &operator=(Expr &&) = default;
    Expr(const Expr &) = default;
    Expr &operator=(const Expr &) = default;    
    
protected:        
    ExprType _type;
    ExprKind _kind;        
};

struct STLE_API ExprHash {
    size_t operator()(const Expr &expr) const {
        U::Hasher hasher;
        expr.hash(hasher);
        return hasher.hash();
    }
};

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const Expr &expr) {
    U::VPrinterRef<TPrinter> pRef(p);
    expr.print(pRef);    
    return p;
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const Expr *expr) {
    return p << *expr;
}

STLE_API std::ostream &operator<<(std::ostream &os, const Expr &expr);

STLE_API std::ostream &operator<<(std::ostream &os, const Expr *expr);

}

#endif
