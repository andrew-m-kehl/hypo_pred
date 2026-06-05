#ifndef STLE_CMD_STRING_RESULT_HPP
#define STLE_CMD_STRING_RESULT_HPP

#include "Stle/Api.hpp"

#include "Result.hpp"

#include "Stle/U/SLexer.hpp"

namespace Stle {
namespace Cmd {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API StringResult : public Result
{
public:
    explicit StringResult(const char *value) : Result(RESULT_STRING), _value(value), _escapedValue(U::escapeSStr(value)) {        
    }
    
    const std::string &value() const {
        return _value;
    }
    
    const std::string &escapedValue() const {
        return _escapedValue;
    }
    
    bool equals(const Result &other) const override;
    
    void print(U::VPrinter &p) const override;
    
private:
    std::string _value;
    std::string _escapedValue;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}
}

#endif
