#ifndef U_COUNTER_HPP
#define U_COUNTER_HPP

#include "Api.hpp"

#include "MemoryU.hpp"

namespace U {

template <typename TValue>
class Counter {
public:
    Counter() : _value(0) {        
    }
    
    void inc(TValue step = 1) {
        _value += step;
    }
    
    TValue value() const {
        return _value;
    }
    
private:
    TValue _value;
};


}

#endif // COUNTER_HPP
