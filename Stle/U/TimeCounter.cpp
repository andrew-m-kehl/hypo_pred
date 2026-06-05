#include "TimeCounter.hpp"

#include "Counters.hpp"

namespace U {

TimeCounter::TimeCounter(int id)
    : TimeCounter(Counters::makeDouble(id)) {        
}

}
