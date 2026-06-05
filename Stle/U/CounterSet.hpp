#ifndef U_COUNTER_SET_HPP
#define U_COUNTER_SET_HPP

#include "Api.hpp"

#include "Counter.hpp"

#include <unordered_map>
#include <string>

namespace U {

template <typename TValue>
struct CounterName {
    Counter<TValue> counter;
    std::string name;
};

template <typename TValue>
class CounterSet {
public:   
    Counter<TValue> &make(int id) {
        return _counters[id].counter;        
    }
    
    void setName(int id, const char *name) {
        _counters[id].name.assign(name);
    }
    
    template <typename TPrinter>
    void print(TPrinter &p);
    
    int size() {
        return static_cast<int>(_counters.size());
    }

private:
    std::unordered_map<int, CounterName<TValue>> _counters;
};

template <typename TValue>
template<typename TPrinter>
void CounterSet<TValue>::print(TPrinter &p) {
    bool first = true;
    for (auto it = _counters.begin(), end = _counters.end(); it != end; ++it) {
        if (!first)
            p << "\n";
        if (!it->second.name.empty())
            p << it->second.name;
        else
            p << "Counter " << it->first;            
        p << ": " << it->second.counter.value();
        first = false;
    }
}

}

#endif
