#ifndef STLE_CMD_CLEAR_MONITOR_CMD_HPP
#define STLE_CMD_CLEAR_MONITOR_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

namespace Stle {
namespace Cmd {

class STLE_API  ClearMonitorCmd : public Cmd {
public:
    ClearMonitorCmd() : Cmd(CMD_CLEAR_MONITOR) {        
    }
        
    bool equals(const Cmd &other) const override;    
};

}
}

#endif
