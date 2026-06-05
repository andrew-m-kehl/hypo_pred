#include "PCSeriesCsvPrinter.hpp"

namespace Stle {

std::ostream &operator<<(std::ostream &os, const PCSeriesCsvPrinter &sp) {
    sp.print(os);
    return os;
}

}
