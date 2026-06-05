#include "SToken.hpp"

namespace U {

std::ostream &operator<<(std::ostream &os, SToken token) {
    printSToken(os, token);    
    return os;
}

}
