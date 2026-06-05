#ifndef STLE_INTVL_END_HPP
#define STLE_INTVL_END_HPP

#include "Api.hpp"

#include "SpaceTime.hpp"

namespace Stle {

class STLE_API IntvlEnd {
public:
    IntvlEnd(Time end, bool closed) : _end(end), _closed(closed) {        
    }
    
    IntvlEnd() : _end(0), _closed(false) {}
    
    Time end() const {
        return _end;
    }
    
    bool closed() const {
        return _closed;
    }
    
    bool operator==(const IntvlEnd &other) const {
        return _end == other._end && _closed == other._closed;
    }
    
    bool operator!=(const IntvlEnd &other) const {
        return !(*this == other);
    }
    
    bool operator<(const IntvlEnd &other) const {
        return _end < other._end || (_end == other._end && !_closed && other._closed);
    }
    
    bool operator>(const IntvlEnd &other) const {
        return _end > other._end || (_end == other._end && _closed && !other._closed);
    }
    
    bool operator<=(const IntvlEnd &other) const {
        return *this < other || *this == other;
    }
    
    bool operator>=(const IntvlEnd &other) const {
        return *this > other || *this == other;
    }
    
private:
    Time _end;
    bool _closed;
};

}

#endif
