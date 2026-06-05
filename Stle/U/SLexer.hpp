#ifndef U_S_LEXER_HPP
#define U_S_LEXER_HPP

#include "Api.hpp"

#include "SToken.hpp"
#include "SLexerError.hpp"
#include "SParserError.hpp"
#include "OfReader.hpp"

#include <sstream>
#include <string>
#include <climits>

namespace U {

U_API std::string escapeSStr(const char *str);

class U_API SCharDescr {
public:
    explicit SCharDescr(int c) : _c(c) {        
    }
    
    int c() const {
        return _c;
    }
    
private:
    int _c;
};

template<typename TPrinter>
void printSCharDescr(TPrinter &p, int c){
    if (c == EOF)
        p << "End of String";
    else
        p << "'" << static_cast<char>(c) << "'";
}

template<typename TPrinter>
void printSCharDescr(TPrinter &p, SCharDescr c){
    printSCharDescr(p, c.c());
}

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, SCharDescr cd) {
    printSCharDescr(p, cd);
    return p;
}

U_API std::ostream &operator<<(std::ostream &os, SCharDescr cd);

// TODO: Perhaps make SLexerRef.
template<class TReader>
class SLexer {
public:    
    template<typename... TArgs>
    explicit SLexer(OfReaderTag, TArgs&&... args)
        : _reader(std::forward<TArgs>(args)...), _token(ST_EMPTY), _tokenCol(-1), _tokenLine(-1), _next(-1), _nextCol(-1), _nextLine(0) {
    }
    
    SToken read();
    
    SToken token() const {
        return _token;
    }
    
    int len() const {
#ifndef NDEBUG
        if (_str.size() > INT_MAX)
            throw std::runtime_error("Buffer size greater than INT_MAX.");
#endif
        return static_cast<int>(_str.size());
    }
    
    const std::string &str() const {
        return _str;
    }
    
    /*! \brief Column where the last read token starts. */
    int col() const {
        return _tokenCol;
    }
    
    /*! \brief Line where the last read token starts. */
    int line() const {
        return _tokenLine;
    }

    // Parser helper functions.
    [[noreturn]] void throwParserError(const std::string &msg);
    void expectToken(SToken expected);
    
private:
    static bool whitespace(int c);
    static bool digit(int c);
    static bool delimeter(int c);
    static bool e(int c);    
    
    int getc();
    int ungetc(int c);
    void continueSymbol();
    void continueInt();
    void continueFrac();
    void continueExp();
    void continueString();
    void continueStringEscape();
    [[noreturn]] void throwError(const char *msg);
    
    TReader _reader;    
    SToken _token;        
    std::string _str;
    int _tokenCol;
    int _tokenLine;
    int _next;
    
    int _nextCol;
    int _nextLine;
};

template<class TReader>
bool SLexer<TReader>::whitespace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r';
}

template<class TReader>
bool SLexer<TReader>::digit(int c) {
    return c >= '0' && c <= '9'; 
}

template<class TReader>
bool SLexer<TReader>::delimeter(int c) {
    return c == EOF || c == '(' || c == ')' || whitespace(c);
}

template<class TReader>
bool SLexer<TReader>::e(int c) {
    return c == 'e' || c == 'E';
}

template<class TReader>
int SLexer<TReader>::getc() {
    int c = _reader.getc();
    if (c == '\n') {
        ++_nextLine;
        _nextCol = 0;        
    }
    else
        ++_nextCol;
    return c;
}

template<class TReader>
int SLexer<TReader>::ungetc(int c) {    
    if (c == '\n') {
        // NOTE: No way and no need to restore _nextCol, since getc will set it to 0 after reading '\n'.
        --_nextLine;
        _nextCol = -1;
    }
    else
        --_nextCol;
    return _reader.ungetc(c);    
}

template<class TReader>
SToken SLexer<TReader>::read() {
    _str.clear();
    _token = ST_EMPTY;
    
    _next = getc();    
    
    _tokenCol = _nextCol;
    _tokenLine = _nextLine;    
    
    bool needUngetc = true;
    
    while(true) {
        if (_next == EOF) {
            _token = ST_EOF;
            return _token;
        }
        if (!whitespace(_next))
            break;
        _next = getc();
    }    
    
    if (_next == '(') {
        _str.push_back(static_cast<char>(_next));
        _token = ST_LP;
        needUngetc = false;
    }
    else if (_next == ')') {
        _str.push_back(static_cast<char>(_next));
        _token = ST_RP;
        needUngetc = false;
    }
    else if (_next == '-') {
        _str.push_back(static_cast<char>(_next));
        _next = getc();
        
        if (digit(_next))
            continueInt();
        else if (_next == '.') {
            _str.push_back(static_cast<char>(_next));
            _next = getc();            
            if (digit(_next)) {
                continueFrac();
            } else
                continueSymbol();
        }
        else
            continueSymbol();        
    }
    else if (_next == '.') {
        _str.push_back(static_cast<char>(_next));
        _next = getc();        
        if (digit(_next)) {
            continueFrac();
        } else
            continueSymbol();            
    }
    else if (_next == '\"') {
        continueString();
    }
    else if (digit(_next)) {        
        continueInt();
    }
    else {
        continueSymbol();
    }
    
    if (_next != EOF && needUngetc) {
        ungetc(_next);
        _next = EOF;
    }
    
    return _token;
}

/*! \brief Reads the remaining part of a symbol.

    Buffer should be non-empty or the next character should be a symbol character.*/
template<class TReader>
void SLexer<TReader>::continueSymbol() {
#ifndef NDEBUG
    if (_str.empty() && delimeter(_next))
        throwError("Buffer should be non-empty or the next character should be a symbol character.");
#endif
    
    _token = ST_SYMBOL;        
    
    while(true) {        
        if (delimeter(_next))
            break;        
        _str.push_back(static_cast<char>(_next));
        _next = getc();
    }
}

/*! \brief Reads a number starting with the integer part.
    
    The next character should be a digit. */
template<class TReader>
void SLexer<TReader>::continueInt() {
#ifndef NDEBUG
    if (!digit(_next))
        throwError("Next character should be a digit.");
#endif    
    
    _token = ST_NUMBER;
    
    while (true) {
        if (delimeter(_next)) {            
            break;
        }
        else if (digit(_next)) {
            _str.push_back(static_cast<char>(_next));
            _next = getc();
        }
        else if (_next == '.') {
            _str.push_back(static_cast<char>(_next));
            _next = getc();
            continueFrac();
            break;
        }
        else if (e(_next)) {
            continueExp();
        }
        else {
            std::stringstream ss;
            ss << "Unexpected character in the integer part: " << SCharDescr(_next) << ".";
            throwError(ss.str().c_str());
        }
    }
}

/*! \brief Reads an optional fractional part of a number. */
template<class TReader>
void SLexer<TReader>::continueFrac() {
    // Number can start with the fractional part, in this case need to set the token type here.
    _token = ST_NUMBER;
    // The dot is should be consumed by the caller.
    while (true) {
        if (delimeter(_next)) {            
            break;
        }
        else if (digit(_next)) {
            _str.push_back(static_cast<char>(_next));
            _next = getc();
        }
        else if (e(_next)) {
            continueExp();
        }
        else {
            std::stringstream ss;
            ss << "Unexpected character in the fractional part: " << SCharDescr(_next) << ".";
            throwError(ss.str().c_str());
        }
    }
}

template<class TReader>
void SLexer<TReader>::continueExp() {
#ifndef NDEBUG
    if (!e(_next))
        throwError("Next symbol should be 'e' or 'E'.");
    if (_token != ST_NUMBER)
        throwError("Caller should set token type to number.");
#endif
    // Next character should be 'e' or 'E'.
    _str.push_back(static_cast<char>(_next));
    _next = getc();
    
    if (_next == '-') {
        _str.push_back(static_cast<char>(_next));
        _next = getc();
    }
    
    if (!digit(_next)) {
        std::stringstream ss;
        ss << "Unexpected character in the exponent: " << SCharDescr(_next) << ".";
        throwError(ss.str().c_str());
    }
    
    while(true) {
        if (delimeter(_next)) {            
            break;
        }
        else if (digit(_next)) {
            _str.push_back(static_cast<char>(_next));
            _next = getc();
        }
        else {
            std::stringstream ss;
            ss << "Unexpected character in the exponent: " << SCharDescr(_next) << ".";
            throwError(ss.str().c_str());
        }
    }
}

template<class TReader>
void SLexer<TReader>::continueString() {
#ifndef NDEBUG
    if (_next != '\"')
        throwError("Next character should be '\"' (double quote).");    
#endif    
    _token = ST_STRING;
    
    // Next character should be double quote, skipping it.
    _next = getc();
    
    while(true) {
        switch (_next) {
        case '\"':            
            _next = getc();
            return;
            
        case '\\':
            continueStringEscape();
            break;
            
        case EOF:
            throwError("End of file while reading a string.");
            
        default:
            _str.push_back(static_cast<char>(_next));
            _next = getc();
            break;
        }
    }
}

template<class TReader>
void SLexer<TReader>::continueStringEscape() {
#ifndef NDEBUG
    if (_next != '\\')
        throwError("Next character should be '\\' (backslash).");
#endif    
    // Next character should be backslash, skipping it.
    _next = getc();    
    switch(_next) {
    case '\\':
        _str.push_back('\\');
        break;
        
    case '\"':
        _str.push_back('\"');
        break;
        
    case 'n':
        _str.push_back('\n');
        break;
        
    case 't':
        _str.push_back('\t');
        break;
        
    case EOF:
        throwError("End of file while reading an escape sequence.");    
        
    default:
        std::stringstream ss;
        ss << "Invalid escape character: " << SCharDescr(_next) << ".";
        throwError(ss.str().c_str());
    }    
    _next = getc();            
}

template<class TReader>
void SLexer<TReader>::throwError(const char *msg) {
    _token = ST_ERROR;
    throw SLexerError(msg);
}

template<class TReader>
void SLexer<TReader>::throwParserError(const std::string &msg) {
    std::stringstream ss;
    ss << "Line " << line()+1 << " char " << col() + 1 << ": " << msg;
    throw SParserError(ss.str());
}

template<class TReader>
void SLexer<TReader>::expectToken(SToken expected) {
    U::SToken found = read();
    if (found != expected) {
        std::stringstream ss;
        ss << "Expected token: " << expected << ", but found: " << found << ".";
        throwParserError(ss.str());
    }
}

}

#endif
