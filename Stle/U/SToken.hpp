#ifndef U_S_TOKEN_HPP
#define U_S_TOKEN_HPP

#include "Api.hpp"

#include <stdexcept>
#include <ostream>

namespace U {

enum SToken {
    ST_EMPTY,    
    ST_LP,
    ST_RP,
    ST_NUMBER,
    ST_SYMBOL,
    ST_STRING,
    ST_ERROR,    
    ST_EOF    
};

template <typename TPrinter>
void printSToken(TPrinter &p, SToken token){
    switch(token) {
    case ST_LP:
        p << "(";
        break;
   
    case ST_RP:
        p << ")";
        break;
        
    case ST_EMPTY:
        p << "Empty";
        break;
        
    case ST_NUMBER:
        p << "Number";
        break;
        
    case ST_SYMBOL:
        p << "Symbol";
        break;
        
    case ST_STRING:
        p << "String";
        break;
        
    case ST_EOF:
        p << "End of File";
        break;
        
    case ST_ERROR:
        p << "Error";
        break;
        
    default:
        throw std::runtime_error("Unknown token type.");
    }    
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &os, SToken token) {
    printSToken(os, token);    
    return os;
}

U_API std::ostream &operator<<(std::ostream &os, SToken token);

}

#endif
