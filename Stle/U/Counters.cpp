#include "Counters.hpp"

namespace U {

Counters &Counters::instance() {
    static Counters _instance;
    return _instance;    
}



}
