#ifndef U_TIMER_COUNTER_HPP
#define U_TIMER_COUNTER_HPP

#include "Api.hpp"

#include "Counter.hpp"

#include <chrono>

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class U_API TimeCounter {
public:
    explicit TimeCounter(Counter<double> &counter)
        : _counter(&counter), _startTime(std::chrono::high_resolution_clock::now()) {        
    }
    
    explicit TimeCounter(int id);
    
    ~TimeCounter() {
        std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - _startTime;
        _counter->inc(duration.count());
    }
    
private:
    Counter<double> *_counter;
    std::chrono::high_resolution_clock::time_point _startTime;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

}

#endif
