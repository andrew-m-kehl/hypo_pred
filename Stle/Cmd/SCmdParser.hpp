#ifndef STLE_CMD_S_CMD_PARSER_HPP
#define STLE_CMD_S_CMD_PARSER_HPP

#include "Stle/Api.hpp"

#include "CmdAll.hpp"

#include "Stle/ExprSet.hpp"
#include "Stle/SExprParser.hpp"

#include "Stle/U/SLexer.hpp"

namespace Stle {
namespace Cmd {

template <typename TLexer>
class SCmdParser {
public:
    template<typename... TArgs>
    SCmdParser(ExprSet &exprSet, TArgs&&... args) : _exprSet(&exprSet), _lexer(std::forward<TArgs>(args)...) {
    }
        
    std::unique_ptr<Cmd> parse() {
        U::SToken token = this->_lexer.read();
        if (token == U::ST_LP) {
            token = this->_lexer.read();
            if (token != U::ST_SYMBOL) {
                std::stringstream ss;
                // TODO: Print also type of token.
                ss << "Expected command symbol, but found '" << this->_lexer.str() << "'.";
                _lexer.throwParserError(ss.str());
            }
            std::unique_ptr<Cmd> cmd;
            if (!_lexer.str().compare("read-signal-csv")) {
                _lexer.expectToken(U::ST_STRING);
                cmd = std::make_unique<ReadSignalCsvCmd>(this->_lexer.str());
            }
            else if (!_lexer.str().compare("eval")) {
                cmd = std::make_unique<EvalCmd>(parseSExprLexer(*_exprSet, this->_lexer));
            }
            else if (!_lexer.str().compare("eval-file")) {
                _lexer.expectToken(U::ST_STRING);
                cmd = std::make_unique<EvalFileCmd>(this->_lexer.str());
            }
            else if (!_lexer.str().compare("eval-r")) {
                cmd = std::make_unique<EvalRCmd>(parseSExprLexer(*_exprSet, this->_lexer));
            }
            else if (!_lexer.str().compare("eval-file-r")) {
                _lexer.expectToken(U::ST_STRING);
                cmd = std::make_unique<EvalFileRCmd>(this->_lexer.str());
            }
            else if (!_lexer.str().compare("clear-monitor")) {
                cmd = std::make_unique<ClearMonitorCmd>();
            }
            else if (!_lexer.str().compare("version")) {
                cmd = std::make_unique<VersionCmd>();
            }
            else if (!_lexer.str().compare("set-read-signal-flags")) {
                _lexer.expectToken(U::ST_SYMBOL);
                cmd = std::make_unique<SetReadSignalFlagsCmd>(readSignalFlagsFromStr(this->_lexer.str().c_str()));
            }
            else if (!_lexer.str().compare("eof")) {
                cmd = std::make_unique<EofCmd>();
            }
            else {
                std::stringstream ss;
                ss << "Unknown command symbol '" << this->_lexer.str() << "'.";
                _lexer.throwParserError(ss.str());
            }
            _lexer.expectToken(U::ST_RP);
            return cmd;
        }
        else if (token == U::ST_EOF) {
            return std::make_unique<EofCmd>();
        }
        else {         
            std::stringstream ss;
            ss << "Expected command expression, but found '" << this->_lexer.str() << "'." ;
            _lexer.throwParserError(ss.str());
        }
    }
    
private:    
    ExprSet *_exprSet;
    TLexer _lexer;
};

}
}

#endif
