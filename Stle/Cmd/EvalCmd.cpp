#include "EvalCmd.hpp"

namespace Stle {
namespace Cmd {

bool EvalCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const EvalCmd &otherEval = static_cast<const EvalCmd&>(other);
    return _expr == otherEval._expr;
}

}
}
