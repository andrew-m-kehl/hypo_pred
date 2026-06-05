#include "ErrorResult.hpp"

namespace Stle {
namespace Cmd {

bool ErrorResult::equals(const Result &other) const {
    if (_type != other.type())
        return false;
    
    const ErrorResult otherError = static_cast<const ErrorResult&>(other);
    return _msg == otherError._msg;
}

void ErrorResult::print(U::VPrinter &p) const {
    p.printf("(error \"%s\")", _escapedMsg.c_str());
}

}
}
