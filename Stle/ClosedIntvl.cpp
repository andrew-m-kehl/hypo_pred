#include "ClosedIntvl.hpp"

namespace Stle {

void ClosedIntvl::hash(U::Hasher &hasher) const {
    hasher.add(_start);
    hasher.add(_end);
}

ClosedIntvl ClosedIntvl::neg(const ClosedIntvl &intvl) {
    return ClosedIntvl(-intvl.end(), -intvl.start());
}

}
