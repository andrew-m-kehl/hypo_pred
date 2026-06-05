#include "ArgReader.hpp"

namespace U {

std::ostream &operator<<(std::ostream &os, ArgToken token) {
    printArgToken(os, token);
    return os;
}

ArgToken ArgReader::read(bool allowKey) {
    if (_argI >= _argc)
        return ARG_EOF;
    ++_argI;
    
    if (_argI >= _argc)
        _token = ARG_EOF;        
    else if (allowKey && _argv[_argI][0] == '-')
        _token = ARG_KEY;
    else
        _token = ARG_VALUE;
    
    return _token;        
}

int ArgReader::readInt() {
    ArgToken token = read(false);        
    
    char *end = nullptr;
    long int l = strtol(str(), &end, 10);               
    
    if (end == str()) {
        std::stringstream ss;
        ss << "Expected integer, but found: ";
        argToStream(ss, token, str());
        throw ArgError(ss.str());
    }
    
    return static_cast<int>(l);
}

double ArgReader::readDouble() {
    ArgToken token = read(false);        
    
    char *end = nullptr;
    double d = strtod(str(), &end);               
    
    if (end == str()) {
        std::stringstream ss;
        ss << "Expected double, but found: ";
        argToStream(ss, token, str());
        throw ArgError(ss.str());
    }
    
    return d;
}

const char *ArgReader::readStr() {
    ArgToken token = read(false);
    if (token != ARG_VALUE) {
        std::stringstream ss;
        ss << "Expected string, but found: ";
        argToStream(ss, token, str());
        throw ArgError(ss.str());
    }
    return str();
}

void ArgReader::argToStream(std::ostream &os, ArgToken token, const char *str) {
    if (token == ARG_KEY || token == ARG_VALUE)
        os << "'" << std::string(str) << "'";
    else
        os << token;        
}

}
