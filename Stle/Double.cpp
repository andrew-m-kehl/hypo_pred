#include "Double.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, const Double &v) {
    v.print(os);
    return os;
}

} 
