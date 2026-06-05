#ifndef STLE_CMD_READ_SIGNAL_CSV_CMD_HPP
#define STLE_CMD_READ_SIGNAL_CSV_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

#include <string>

namespace Stle {
namespace Cmd {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class STLE_API ReadSignalCsvCmd : public Cmd {
public:
    ReadSignalCsvCmd(const char *fileName) : Cmd(CMD_READ_SIGNAL_CSV), _fileName(fileName) {        
    }
    
    ReadSignalCsvCmd(const std::string &fileName) : Cmd(CMD_READ_SIGNAL_CSV), _fileName(fileName) {        
    }    
    
    const std::string &fileName() const {
        return _fileName;
    }
    
    bool equals(const Cmd &other) const override;

private:
    std::string _fileName;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}
}

#endif
