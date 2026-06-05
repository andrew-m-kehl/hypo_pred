#ifndef STLE_CMD_EVAL_FILE_CMD_HPP
#define STLE_CMD_EVAL_FILE_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

#include <string>

namespace Stle {
namespace Cmd {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API EvalFileCmd : public Cmd {
public:
   EvalFileCmd(std::string fileName) : Cmd(CMD_EVAL_FILE), _fileName(std::move(fileName))  {
    }
    
    const std::string &fileName() const {
        return _fileName;
    }
    
    bool equals(const Cmd &other) const override;
        
private:
    const std::string _fileName;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}
}

#endif
