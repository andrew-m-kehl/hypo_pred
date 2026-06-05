#ifndef STLE_DOUBLE_HPP
#define STLE_DOUBLE_HPP

#include "Api.hpp"

#include "U/HashU.hpp"

#include <type_traits>
#include <ostream>
#include <limits>

namespace Stle {

class STLE_API Double {
public:
    Double() {        
    }
    
    explicit constexpr Double(double value) : _value(value) {        
    }
    
    constexpr double value() const {
        return _value;
    }   
    
    void hash(U::Hasher &hasher) const {
        hasher.add(_value);
    }
    
    constexpr bool operator==(const Double &other) const {
        return _value == other._value;
    }
    
    constexpr bool operator!=(const Double &other) const {
        return !(*this == other);
    }
    
    constexpr bool operator<(const Double &other) const {
        return _value < other._value;
    }
    
    constexpr bool operator<=(const Double &other) const {
        return _value <= other._value;
    }
    
    constexpr bool operator>(const Double &other) const {
        return _value > other._value;
    }
    
    constexpr bool operator>=(const Double &other) const {
        return _value >= other._value;
    }
    
    constexpr Double operator-() const {
        return Double(-_value);
    }
    
    constexpr Double operator-(const Double &other) const {
        return Double(_value - other._value);
    }
    
    constexpr Double &operator-=(const Double &other) {        
        _value -= other._value;
        return *this;
    }
    
    constexpr Double operator+(const Double &other) const {
        return Double(_value + other._value);
    }
    
    constexpr Double &operator+=(const Double &other) {        
        _value += other._value;
        return *this;
    }
    
    constexpr Double operator*(const Double &other) const {
        return Double(_value * other._value);
    }
    
    constexpr Double &operator*=(const Double &other) {        
        _value *= other._value;
        return *this;
    }
    
    constexpr Double operator/(const Double &other) const {
        return Double(_value / other._value);
    }
    
    constexpr Double &operator/=(const Double &other) {        
        _value /= other._value;
        return *this;
    }
    
    static constexpr Double inf() {
        return Double(std::numeric_limits<double>::infinity());
    }
    
    static constexpr Double minusInf() {
#ifndef NDEBUG
        if (!std::numeric_limits<double>::is_iec559)
            throw std::runtime_error("Cannot express negative infinity");
#endif
        return Double(-std::numeric_limits<double>::infinity());
    }
    
    template <typename TPrinter>
    void print(TPrinter &p) const {
        p << _value;
    }
    
private:
    double _value;
};

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const Double &v) {
    v.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, const Double &v);

} 

#endif
