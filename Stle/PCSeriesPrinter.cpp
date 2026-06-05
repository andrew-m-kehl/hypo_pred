#include "PCSeriesPrinter.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &p, const PCSeriesPrinter &sp) {
    sp.print(p);
    return p;
}


}
