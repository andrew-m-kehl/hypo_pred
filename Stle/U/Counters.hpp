#ifndef U_COUNTERS_HPP
#define U_COUNTERS_HPP

#include "Api.hpp"

#include "CounterSet.hpp"

namespace U {

#ifdef _MSC_VER
    #pragma warning(disable: 4251)
#endif
class U_API Counters {
public:
    
    static Counter<int> &makeInt(int id) {
        return instance()._intCounters.make(id);
    }
    
    static Counter<double> &makeDouble(int id) {
        return instance()._doubleCounters.make(id);
    }
    
    static void setNameInt(int id, const char *name) {
        instance()._intCounters.setName(id, name);
    }
    
    static void setNameDouble(int id, const char *name) {
        instance(). _doubleCounters.setName(id, name);
    }
    
    template <typename TPrinter>
    static void print(TPrinter &p);
    
private:
    Counters() {}
    static Counters &instance();
    
    CounterSet<int> _intCounters;
    CounterSet<double> _doubleCounters;
};
#ifdef _MSC_VER
    #pragma warning(default: 4251)
#endif

template<typename TPrinter>
void Counters::print(TPrinter &p) {
    instance()._intCounters.print(p);
    if (instance()._intCounters.size() > 0)
        p << "\n";
    instance()._doubleCounters.print(p);
    if (instance()._doubleCounters.size() > 0)
        p << "\n";
}

}

#endif
