#include "EvalRCmd.hpp"

namespace Stle {
namespace Cmd {

bool EvalRCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const EvalRCmd &otherEvalR = static_cast<const EvalRCmd&>(other);
    return _expr == otherEvalR._expr;
}

}
}
