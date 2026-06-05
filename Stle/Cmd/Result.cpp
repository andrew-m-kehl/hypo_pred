#include "Result.hpp"

#include "Stle/U/VOstreamRef.hpp"

namespace Stle {
namespace Cmd {

std::ostream &operator<<(std::ostream &os, const Result &expr) {
    U::VOstreamRef osRef(os);
    expr.print(osRef);
    return os;
}

}
} 
