#ifndef STLE_CLOSED_INTVL_HPP
#define STLE_CLOSED_INTVL_HPP

#include "Api.hpp"

#include "SpaceTime.hpp"

#include "U/HashU.hpp"

#include <stdexcept>

namespace Stle {

// TODO: Maybe make this a template.
class STLE_API ClosedIntvl {
public:
    constexpr ClosedIntvl(Time start, Time end) : _start(start), _end(end) {
        if (start > end)
            throw std::runtime_error("Start time should not be after end time.");
    }
    
    constexpr ClosedIntvl(int start, int end) : ClosedIntvl(Time(start), Time(end)) {        
    }
    
    constexpr ClosedIntvl(double start, double end) : ClosedIntvl(Time(start), Time(end)) {        
    }
    
    static constexpr ClosedIntvl zeroToInf() {
        return ClosedIntvl(Time(0), Time::inf());
    }
    
    static constexpr ClosedIntvl minusInfToZero() {
        return ClosedIntvl(Time::minusInf(), Time(0));
    }
    
    bool future() const {
        return _start >= Time(0);
    }
    
    bool past() const {
        return !future();
    }
    
    Time start() const {
        return _start;
    }
    
    Time end() const {
        return _end;
    }
    
    bool operator==(const ClosedIntvl &other) const {
        return _start == other._start && _end == other._end;
    }
    
    bool operator!=(const ClosedIntvl &other) const {
        return !(*this == other);
    }
    
    void hash(U::Hasher &hasher) const;
    
    template <typename TPrinter>
    void print(TPrinter &p) const;
    
    static ClosedIntvl neg(const ClosedIntvl &intvl);
    
private:
    Time _start;
    Time _end;
};

template <typename TPrinter>
void ClosedIntvl::print(TPrinter &p) const {
    if (_start != _end)
        p << "[" << _start << ", " << _end << "]";
    else
        p << "[" << _start << "]";        
}

template <typename TPrinter>
std::ostream &operator<<(TPrinter &p, const ClosedIntvl &intvl) {
    intvl.print(p);
    return p;
}

}

#endif
