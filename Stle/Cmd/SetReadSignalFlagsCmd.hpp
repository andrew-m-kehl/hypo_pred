#ifndef STLE_CMD_SET_READ_SIGNAL_FLAGS_CMD_HPP
#define STLE_CMD_SET_READ_SIGNAL_FLAGS_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

#include "Stle/ReadSignalFlags.hpp"

namespace Stle {
namespace Cmd {

class STLE_API SetReadSignalFlagsCmd : public Cmd {
public:
    SetReadSignalFlagsCmd(ReadSignalFlags flags) : Cmd(CMD_SET_READ_SIGNAL_FLAGS), _flags(flags) {        
    }
    
    ReadSignalFlags flags() const {
        return _flags;
    }
    
    bool equals(const Cmd &other) const override;
    
private:
    ReadSignalFlags _flags;    
};

}
}

#endif
