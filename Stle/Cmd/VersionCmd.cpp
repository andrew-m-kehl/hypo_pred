#include "VersionCmd.hpp"

namespace Stle {
namespace Cmd {

bool VersionCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    return true;
}

}
}
