#include "EvalFileCmd.hpp"

namespace Stle {
namespace Cmd {

bool EvalFileCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const EvalFileCmd &otherEvalFile = static_cast<const EvalFileCmd&>(other);
    return _fileName == otherEvalFile._fileName;
}

}
}
