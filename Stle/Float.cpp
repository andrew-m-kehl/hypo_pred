#include "Float.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, const Float &v) {
    v.print(os);
    return os;
}

}
