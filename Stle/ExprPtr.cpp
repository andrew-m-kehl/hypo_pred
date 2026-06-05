#include "ExprPtr.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, const ExprPtr &expr) {
    return os << *expr.get();
}

}
