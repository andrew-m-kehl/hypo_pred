#ifndef STLE_CMD_ERROR_RESULT_HPP
#define STLE_CMD_ERROR_RESULT_HPP

#include "Stle/Api.hpp"

#include "Result.hpp"

#include "Stle/U/SLexer.hpp"

#include <string>

namespace Stle {
namespace Cmd {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API ErrorResult : public Result {
public:
    explicit ErrorResult(const char *msg) : Result(RESULT_ERROR), _msg(msg), _escapedMsg(U::escapeSStr(msg)) {        
    }
    
    bool equals(const Result &other) const override;
    
    void print(U::VPrinter &p) const override;
    
private:
    std::string _msg;
    std::string _escapedMsg;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}
} 

#endif
