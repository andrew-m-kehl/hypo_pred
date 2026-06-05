#include "SetReadSignalFlagsCmd.hpp"

namespace Stle {
namespace Cmd {

bool SetReadSignalFlagsCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const SetReadSignalFlagsCmd &otherSetRsf = static_cast<const SetReadSignalFlagsCmd&>(other);
    return _flags == otherSetRsf._flags;
}



}
}
