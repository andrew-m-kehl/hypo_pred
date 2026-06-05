#ifndef STLE_PC_SEG_HPP
#define STLE_PC_SEG_HPP

#include "Api.hpp"

#include "SpaceTime.hpp"

namespace Stle {

class STLE_API PCSeg {
public:
    PCSeg() : _endTime(0), _value(0) {
    }

    PCSeg(Time endTime, Space value)
        : _endTime(endTime), _value(value) {
    }         
    
    Time endTime() const {
        return _endTime;
    }
    
    Space value() const {
        return _value;
    }
    
    bool operator==(const PCSeg &other) const {
        return _endTime == other.endTime() && _value == other.value();
    }
    
    bool operator!=(const PCSeg &other) const {
        return !(*this == other);
    }
    
private:
    // Since Time is normally double, having space less than 8 bytes does not help much.
    // Having Float for Space is only marginally faster, about 10% faster with pragma pack 1.
    // Having Float for Time as well is about 25% faster.
    Time _endTime;
    Space _value;
};

}

#endif
