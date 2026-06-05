#ifndef STLE_CMD_EOF_CMD_HPP
#define STLE_CMD_EOF_CMD_HPP

#include "Stle/Api.hpp"

#include "Cmd.hpp"

namespace Stle {
namespace Cmd {

class STLE_API EofCmd : public Cmd {
public:
    EofCmd() : Cmd(CMD_EOF) {        
    }
       
    bool equals(const Cmd &other) const override;
    
};

}
}

#endif
