#ifndef STLE_FLOAT_HPP
#define STLE_FLOAT_HPP

#include "Api.hpp"

#include "U/HashU.hpp"

#include <type_traits>
#include <ostream>
#include <limits>

namespace Stle {

class STLE_API Float {
public:
    Float() {        
    }
    
    explicit Float(float value) : _value(value) {        
    }
    
    double value() const {
        return _value;
    }   
    
    void hash(U::Hasher &hasher) const {
        hasher.add(_value);
    }
    
    bool operator==(const Float &other) const {
        return _value == other._value;
    }
    
    bool operator!=(const Float &other) const {
        return !(*this == other);
    }
    
    bool operator<(const Float &other) const {
        return _value < other._value;
    }
    
    bool operator<=(const Float &other) const {
        return _value <= other._value;
    }
    
    bool operator>(const Float &other) const {
        return _value > other._value;
    }
    
    bool operator>=(const Float &other) const {
        return _value >= other._value;
    }
    
    Float operator-() const {
        return Float(-_value);
    }
    
    Float operator-(const Float &other) const {
        return Float(_value - other._value);
    }
    
    Float &operator-=(const Float &other) {        
        _value -= other._value;
        return *this;
    }
    
    Float operator+(const Float &other) const {
        return Float(_value + other._value);
    }
    
    Float &operator+=(const Float &other) {        
        _value += other._value;
        return *this;
    }
    
    Float operator*(const Float &other) const {
        return Float(_value * other._value);
    }
    
    Float &operator*=(const Float &other) {        
        _value *= other._value;
        return *this;
    }
    
    Float operator/(const Float &other) const {
        return Float(_value / other._value);
    }
    
    Float &operator/=(const Float &other) {        
        _value /= other._value;
        return *this;
    }
    
    static Float infinity() {
        return Float(std::numeric_limits<float>::infinity());
    }
    
    static Float minusInfinity() {
#ifndef NDEBUG
        if (!std::numeric_limits<double>::is_iec559)
            throw std::runtime_error("Cannot express negative infinity");
#endif
        return Float(-std::numeric_limits<float>::infinity());
    }
    
    template <typename TPrinter>
    void print(TPrinter &p) const {
        p << _value;
    }
    
private:
    float _value;
};

template <typename TPrinter>
TPrinter &operator<<(TPrinter &p, const Float &v) {
    v.print(p);
    return p;
}

STLE_API std::ostream &operator<<(std::ostream &os, const Float &v);

}

#endif
