#include "SLexer.hpp"

namespace U {

std::string escapeSStr(const char *str) {
    std::string result;
    size_t i = 0;
    while(true) {
        char c = str[i];
        if (c == '\0')
            break;
        
        switch (c) {        
        case '\"':
            result.push_back('\\');
            result.push_back('\"');
            break;
            
        case '\\':
            result.push_back('\\');
            result.push_back('\\');
            break;
            
        case '\n':
            result.push_back('\\');
            result.push_back('n');
            break;
            
        case '\t':
            result.push_back('\\');
            result.push_back('t');
            break;
            
        default:
            result.push_back(c);
            break;
        }
        
        ++i;
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, SCharDescr cd) {
    printSCharDescr(os, cd);
    return os;
}

}
