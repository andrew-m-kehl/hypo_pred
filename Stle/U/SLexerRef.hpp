#ifndef U_S_LEXER_REF_HPP
#define U_S_LEXER_REF_HPP

#include "Api.hpp"

#include "SToken.hpp"

namespace U {

template <typename TLexer>
class SLexerRef {
public:
    SLexerRef(TLexer &lexer) : _lexer(&lexer) {        
    }
    
    SToken read() {
        return _lexer->read();
    }
    
    SToken token() {
        return _lexer->token();
    }
    
    int len() const {
        return _lexer->len();
    }
    
    const std::string &str() const {
        return _lexer->str();
    }
    
    int col() const {
        return _lexer->col();
    }
    
    int line() const {
        return _lexer->line();
    }

    // Parser helper functions.
    [[noreturn]] void throwParserError(const std::string &msg) {
        _lexer->throwParserError(msg);
    }

    void expectToken(SToken expected) {
        _lexer->expectToken(expected);
    }
    
private:
    TLexer *_lexer;
};

}

#endif
