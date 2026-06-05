#ifndef STLE_S_EXPR_PARSER_HPP
#define STLE_S_EXPR_PARSER_HPP

#include "Api.hpp"

#include "ExprAll.hpp"
#include "ExprSet.hpp"

#include "U/SLexer.hpp"
#include "U/SLexerRef.hpp"
#include "U/StrReader.hpp"

#include <utility>
#include <sstream>

namespace Stle {

STLE_API ExprPtr parseSExprStr(ExprSet &exprSet, const char *str, int *pos = nullptr);

template <typename TLexer>
ExprPtr parseSExprLexer(ExprSet &exprSet, TLexer &lexer);

template <typename TLexer>
class SExprParser {
public:
    template<typename... TArgs>
    SExprParser(ExprSet &exprSet, TArgs&&... args) : _exprSet(&exprSet), _lexer(std::forward<TArgs>(args)...) {
    }
    
    ExprPtr parse();
    
private:    
    ExprPtr parsePointExpr(bool allowRP = false);
    ExprPtr parseIntvlExpr(bool untilArg, bool allowRP = false);
    ExprPtr parseApply(bool intvlExpr, bool untilArg);
    std::vector<ExprPtr> parsePointExprList();
    std::vector<ExprPtr> parseIntvlExprList(bool untilArg);
    
    ClosedIntvl parseTimeIntvl();
    Time parseTime();
    Space parseSpace();    

    ExprSet *_exprSet;
    TLexer _lexer;
};

template<typename TLexer>
ExprPtr SExprParser<TLexer>::parse() {
    return parsePointExpr();
}

template<typename TLexer>
ExprPtr SExprParser<TLexer>::parsePointExpr(bool allowRP) {
     U::SToken token = _lexer.read();     
     if (token == U::ST_NUMBER) {
         // NOTE: Assuming that the number is parseable as double.
         double value = strtod(_lexer.str().c_str(), nullptr);
         return _exprSet->makeConst(Space(value));
     }
     else if (token == U::ST_SYMBOL) {
         if (!_lexer.str().compare("inf"))
             return _exprSet->makeConst(Space::inf());
         else if (!_lexer.str().compare("-inf"))         
            return _exprSet->makeConst(Space::minusInf());
         else if (!_lexer.str().compare("true"))
             return _exprSet->makeBoolConst(true);
         else if (!_lexer.str().compare("false"))
             return _exprSet->makeBoolConst(false);
         else
            return _exprSet->makeVar(_lexer.str().c_str());
     }
     else if (token == U::ST_LP) {
         token = _lexer.read();
         if (token != U::ST_SYMBOL) {
             std::stringstream ss;
             ss << "Expected temporal operator or function symbol, but found '" << _lexer.str() << "'.";
             _lexer.throwParserError(ss.str());
         }
         if (!_lexer.str().compare("F")) {
             ClosedIntvl intvl = parseTimeIntvl();
             ExprPtr arg = parsePointExpr();
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeTemporal(intvl, TEMPORAL_F, arg);
         }
         else if (!_lexer.str().compare("G")) {
             ClosedIntvl intvl = parseTimeIntvl();
             ExprPtr arg = parsePointExpr();
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeTemporal(intvl, TEMPORAL_G, arg);
         }
         else if (!_lexer.str().compare("On")) {
             ClosedIntvl intvl = parseTimeIntvl();
             ExprPtr arg = parseIntvlExpr(false);
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeOn(intvl, arg);
         }
         else if (!_lexer.str().compare("Until")) {
             ClosedIntvl intvl = parseTimeIntvl();
             Space defaultValue = parseSpace();
             ExprPtr arg1 = parseIntvlExpr(true);
             ExprPtr arg2 = parsePointExpr();
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeUntil(intvl, defaultValue, arg1, arg2);
         } else if (!_lexer.str().compare("Lookup")) {
             Time delay = parseTime();
             Space defaultValue = parseSpace();
             ExprPtr arg = parsePointExpr();
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeLookup(delay, defaultValue, arg);
         }      
         else if (!_lexer.str().compare("StlUntil")) {
             ClosedIntvl intvl = parseTimeIntvl();
             ExprPtr arg1 = parsePointExpr();
             ExprPtr arg2 = parsePointExpr();
             _lexer.expectToken(U::ST_RP);
             return _exprSet->makeTemporalUntil(intvl, arg1, arg2);
         }
         else {
             return parseApply(false, false);        
         }         
     }
     else if (token == U::ST_RP && allowRP) {
         return ExprPtr();
     }
     else {         
         std::stringstream ss;
         ss << "Expected point expression, but found '" << _lexer.str() << "'." ;
         _lexer.throwParserError(ss.str());
     }     
}

template<typename TLexer>
ExprPtr SExprParser<TLexer>::parseApply(bool intvlExpr, bool untilArg) {
#ifndef NDEBUG
    if (_lexer.token() != U::ST_SYMBOL)
        throw std::runtime_error("Current token should be a function symbol.");
#endif
    ApplyOp op = APPLY_NOP;
    if (!_lexer.str().compare("min")) 
        op = APPLY_MIN;
    else if (!_lexer.str().compare("max"))
        op = APPLY_MAX;
    else if (!_lexer.str().compare("D"))
        op = APPLY_DER;
    else if (!_lexer.str().compare("I"))
        op = APPLY_INT;
    else if (!_lexer.str().compare("+"))
        op = APPLY_PLUS;
    else if (!_lexer.str().compare("-"))
        op = APPLY_MINUS;
    else if (!_lexer.str().compare("*"))
        op = APPLY_MUL;
    else if (!_lexer.str().compare("/"))
        op = APPLY_DIV;
    else if (!_lexer.str().compare("and")) 
        op = APPLY_BOOL_AND;
    else if (!_lexer.str().compare("or")) 
        op = APPLY_BOOL_OR;
    else if (!_lexer.str().compare("not")) 
        op = APPLY_BOOL_NOT;
    else if (!_lexer.str().compare("->") || !_lexer.str().compare("=>")) 
        op = APPLY_BOOL_IMPL;
    else if (!_lexer.str().compare("abs")) 
        op = APPLY_ABS;
    else if (!_lexer.str().compare("ite")) 
        op = APPLY_ITE;
    else if (!_lexer.str().compare("<")) 
        op = APPLY_LT;
    else if (!_lexer.str().compare("<=")) 
        op = APPLY_LEQ;
    else if (!_lexer.str().compare(">")) 
        op = APPLY_GT;
    else if (!_lexer.str().compare(">=")) 
        op = APPLY_GEQ;
    else {
        std::stringstream ss;
        ss << "Unknown operator: " << _lexer.str() << "." ;
        _lexer.throwParserError(ss.str());
    }
    
    std::vector<ExprPtr> args;
    if (intvlExpr)
        args = parseIntvlExprList(untilArg);
    else
        args = parsePointExprList();
    
    try {
        return _exprSet->makeApply(op, args.begin(), args.end());
    } catch (ExprError e) {
        _lexer.throwParserError(e.what());
    }     
}

template<typename TLexer>
ExprPtr SExprParser<TLexer>::parseIntvlExpr(bool untilArg, bool allowRP) {
    U::SToken token = _lexer.read();
    if (token == U::ST_LP) {
        token = _lexer.read();
        if (token != U::ST_SYMBOL) {
            std::stringstream ss;
            ss << "Expected temporal operator or function symbol, but found '" << _lexer.str() << "'.";
            _lexer.throwParserError(ss.str());
        }
        if (!_lexer.str().compare("Min")) {
            ExprPtr arg = parsePointExpr();
            _lexer.expectToken(U::ST_RP);
            return _exprSet->makeAggregate(AGGREGATE_MIN, arg);
        }
        else if (!_lexer.str().compare("Max")) {
            ExprPtr arg = parsePointExpr();
            _lexer.expectToken(U::ST_RP);
            return _exprSet->makeAggregate(AGGREGATE_MAX, arg);
        }
        else if (!_lexer.str().compare("I")) {
            ExprPtr arg = parsePointExpr();
            _lexer.expectToken(U::ST_RP);
            return _exprSet->makeAggregate(AGGREGATE_INT, arg);
        }
        else if (!_lexer.str().compare("Get") && untilArg) {
            ExprPtr arg = parsePointExpr();
            _lexer.expectToken(U::ST_RP);
            return _exprSet->makeGet(arg);
        }
        else {
            return parseApply(true, untilArg);
        }
        
    } else if (token == U::ST_RP && allowRP) {
        return ExprPtr();
    }
    else {         
        std::stringstream ss;
        ss << "Expected interval expression, but found " << _lexer.str() << "." ;
        _lexer.throwParserError(ss.str());
    }
}

template<typename TLexer>
ClosedIntvl SExprParser<TLexer>::parseTimeIntvl() {
    _lexer.expectToken(U::ST_LP);
    Time start = parseTime();
    Time end = parseTime();    
    _lexer.expectToken(U::ST_RP);
    
    if (start > end)
        _lexer.throwParserError("Start time should not be greater than end time.");
    
    return ClosedIntvl(start, end);
}

template<typename TLexer>
Time SExprParser<TLexer>::parseTime() {
    Time time(0);
    U::SToken token = _lexer.read();    
    if (token == U::ST_NUMBER)
        // NOTE: Assuming that the number is parseable as double.
        time = Time(strtod(_lexer.str().c_str(), nullptr));
    else if (token == U::ST_SYMBOL && !_lexer.str().compare("inf"))
        time = Time::inf();
    else if (token == U::ST_SYMBOL && !_lexer.str().compare("-inf"))
        time = Time::minusInf();
    else {
        // TODO: Query position from lexer.
        std::stringstream ss;
        ss << "Expected time constant, but found '" << _lexer.str() << "'." ;
        _lexer.throwParserError(ss.str());
    }
    return time;
}

template<typename TLexer>
Space SExprParser<TLexer>::parseSpace() {
    Space space(0);
    U::SToken token = _lexer.read();    
    if (token == U::ST_NUMBER)
        // NOTE: Assuming that the number is parseable as double.
        space = Space(strtod(_lexer.str().c_str(), nullptr));
    else if (token == U::ST_SYMBOL && !_lexer.str().compare("inf"))
        space = Space::inf();
    else if (token == U::ST_SYMBOL && !_lexer.str().compare("-inf"))
        space = Space::minusInf();
    else {
        // TODO: Query position from lexer.
        std::stringstream ss;
        ss << "Expected spatial constant, but found '" << _lexer.str() << "'." ;
        _lexer.throwParserError(ss.str());
    }
    return space;
}

template<typename TLexer>
std::vector<ExprPtr> SExprParser<TLexer>::parsePointExprList() {
    std::vector<ExprPtr> result;
    while (true) {
        ExprPtr expr = parsePointExpr(true);
        if (!expr)
            break;
        result.emplace_back(std::move(expr));
    }
    return result;
}

template<typename TLexer>
std::vector<ExprPtr> SExprParser<TLexer>::parseIntvlExprList(bool untilArg) {
    std::vector<ExprPtr> result;
    while (true) {
        ExprPtr expr = parseIntvlExpr(untilArg, true);
        if (!expr)
            break;
        result.emplace_back(std::move(expr));
    }
    return result;
}

template<typename TLexer>
ExprPtr parseSExprLexer(ExprSet &exprSet, TLexer &lexer) {
    return SExprParser<U::SLexerRef<TLexer>>(exprSet, lexer).parse();
}

}

#endif
