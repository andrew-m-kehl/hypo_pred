#include "ClearMonitorCmd.hpp"

namespace Stle {
namespace Cmd {

bool ClearMonitorCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    return true;
}

}
}
