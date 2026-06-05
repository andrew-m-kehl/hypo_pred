#include "EvalFileRCmd.hpp"

namespace Stle {
namespace Cmd {

bool EvalFileRCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const EvalFileRCmd &otherEvalFileR = static_cast<const EvalFileRCmd&>(other);
    return _fileName == otherEvalFileR._fileName;
}

}
}
