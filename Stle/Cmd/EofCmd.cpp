#include "EofCmd.hpp"

namespace Stle {
namespace Cmd {

bool EofCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    return true;
}

}
}
