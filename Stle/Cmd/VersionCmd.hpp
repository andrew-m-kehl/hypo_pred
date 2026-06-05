#ifndef STLE_CMD_VERSION_CMD
#define STLE_CMD_VERSION_CMD

#include "Stle/Api.hpp"

#include "Cmd.hpp"

namespace Stle {
namespace Cmd {

class STLE_API VersionCmd : public Cmd {
public:
    VersionCmd() : Cmd(CMD_VERSION) {        
    }
    
    bool equals(const Cmd &other) const override;
};

}
}

#endif
