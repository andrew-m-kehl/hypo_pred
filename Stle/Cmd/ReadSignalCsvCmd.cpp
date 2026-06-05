#include "ReadSignalCsvCmd.hpp"

namespace Stle {
namespace Cmd {

bool ReadSignalCsvCmd::equals(const Cmd &other) const {
    if (_type != other.type())
        return false;
    
    const ReadSignalCsvCmd &otherR = static_cast<const ReadSignalCsvCmd&>(other);
    return _fileName == otherR.fileName();
}

}
}
