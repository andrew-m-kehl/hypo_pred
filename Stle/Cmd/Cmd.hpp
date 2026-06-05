#ifndef STLE_CMD_CMD_HPP
#define STLE_CMD_CMD_HPP

#include "Stle/Api.hpp"

#include "State.hpp"

namespace Stle {
namespace Cmd {

enum CmdType {
    CMD_READ_SIGNAL_CSV,
    CMD_EVAL,
    CMD_EVAL_FILE,
    CMD_EVAL_R,
    CMD_EVAL_FILE_R,
    CMD_CLEAR_MONITOR,
    CMD_VERSION,
    CMD_SET_READ_SIGNAL_FLAGS,
    // TODO: Commands for other CLI capabilities.
    CMD_EOF
};

class STLE_API Cmd {    
public:
    Cmd(CmdType type) : _type(type) {        
    }
    
    virtual bool equals(const Cmd &other) const = 0;
        
    // TODO: Perhaps printing.

    CmdType type() const {
        return _type;
    }
        
    bool operator==(const Cmd &other) const {
        return equals(other);
    }
    
    bool operator!=(const Cmd &other) const {
        return !(*this == other);
    }    
    
    virtual ~Cmd() = default;
    // Need to re-enable moving after adding a virtual destructor.
    Cmd(Cmd &&) = default;
    Cmd &operator=(Cmd &&) = default;
    Cmd(const Cmd &) = default;
    Cmd &operator=(const Cmd &) = default;
    
protected:
    CmdType _type;
};

}
}

#endif
