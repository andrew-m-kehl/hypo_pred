#include "SExprParser.hpp"

#include "U/ReaderRef.hpp"

namespace Stle {

ExprPtr parseSExprStr(ExprSet &exprSet, const char *str, int *pos) {
    U::StrReader charReader(str);
    ExprPtr result = SExprParser<U::SLexer<U::ReaderRef<U::StrReader>>>(exprSet, U::OF_READER, charReader).parse();
    if (pos)
        *pos = charReader.i();
    return result;
}

}
