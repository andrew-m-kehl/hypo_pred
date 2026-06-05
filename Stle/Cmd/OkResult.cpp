#include "OkResult.hpp"

namespace Stle {
namespace Cmd {

bool OkResult::equals(const Result &other) const {
    if (_type != other.type())
        return false;
    return true;
}

void OkResult::print(U::VPrinter &p) const {
    p << "ok";
}

}
}
