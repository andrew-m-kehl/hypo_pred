#ifndef STLE_APPLY_OP_HPP
#define STLE_APPLY_OP_HPP

#include "Api.hpp"

#include <ostream>

namespace Stle {

enum ApplyOp {
    APPLY_NOP,
    APPLY_MIN,
    APPLY_MAX,
    APPLY_DER,
    APPLY_INT,
    APPLY_PLUS,
    APPLY_MINUS,    
    APPLY_MUL,
    APPLY_DIV,
    APPLY_BOOL_AND,
    APPLY_BOOL_OR,
    APPLY_BOOL_NOT,
    APPLY_BOOL_IMPL,
    APPLY_ABS,
    APPLY_ITE,
    APPLY_LT,
    APPLY_LEQ,
    APPLY_GT,
    APPLY_GEQ
};

template <typename TPrinter>
void printApplyOp(TPrinter &p, ApplyOp op) {
    switch (op) {
    case APPLY_NOP:
        p << "nop";
        return;
        
    case APPLY_MIN:
        p << "min";
        return;
        
    case APPLY_MAX:
        p << "max";
        return;

    case APPLY_DER:
        p << "D";
        return;

    case APPLY_INT:
        p << "I";
        return;

    case APPLY_PLUS:
        p << "+";
        return;
        
    case APPLY_MINUS:
        p << "-";
        return;
        
    case APPLY_BOOL_AND:
        p << "and";
        return;
        
    case APPLY_BOOL_OR:
        p << "or";
        return;
        
    case APPLY_BOOL_NOT:
        p << "not";
        return;
        
    case APPLY_BOOL_IMPL:
        p << "->";
        return;
        
    case APPLY_ABS:
        p << "abs";
        return;
        
    case APPLY_ITE:
        p << "ite";
        return;
        
    case APPLY_LT:
        p << "<";
        return;
        
    case APPLY_LEQ:
        p << "<=";
        return;
        
    case APPLY_GT:
        p << ">";
        return;
        
    case APPLY_GEQ:
        p << ">=";
        return;
        
    default:
        throw std::runtime_error("Unsupported operator.");        
    }
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, ApplyOp op) {
    printApplyOp(p, op);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, ApplyOp op); 

}

#endif
