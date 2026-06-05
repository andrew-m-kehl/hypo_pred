#ifndef U_HASHER_HPP
#define U_HASHER_HPP

#include "Api.hpp"

#include "Murmur332Hasher.hpp"

#include <stdexcept>
#include <vector>
#include <type_traits>
#include <climits>

namespace U {

class U_API Hasher {
public:
    
    void addBytes(const char *data, size_t len) {
#ifndef NDEBUG
        if (len > INT_MAX)
            throw std::runtime_error("Length should not be greater than INT_MAX");
#endif
        _hasher.addBytes(data, static_cast<int>(len));
    }
    
    template<typename T,
             typename std::enable_if<std::is_enum<T>::value || std::is_integral<T>::value || std::is_pointer<T>::value, int>::type = 0>
    void add(T t) {
        addBytes(reinterpret_cast<const char*>(&t), sizeof (T));
    }
    
    template<typename T,
             typename std::enable_if<!std::is_enum<T>::value && !std::is_integral<T>::value && !std::is_pointer<T>::value, int>::type = 0>
    void add(const T &t) {
        t.hash(*this);
    }
    
    void add(float d) {
        // Converts -0.0 to 0.0.
        d = (d == 0.0f) ? 0.0f : d;
        addBytes(reinterpret_cast<const char*>(&d), sizeof (float));
    }
    
    void add(double d) {
        // Converts -0.0 to 0.0.
        d = (d == 0.0) ? 0.0 : d;
        addBytes(reinterpret_cast<const char*>(&d), sizeof (double));
    }
    
    void add(long double d) {
        // Converts -0.0 to 0.0.
        d = (d == 0.0l) ? 0.0l : d;
        addBytes(reinterpret_cast<const char*>(&d), sizeof (long double));
    }
    
    void add(const std::string &s) {        
        addBytes(s.c_str(), s.length());
    }
    
    template <typename T>
    void add(const std::vector<T> &v) {        
        for (size_t i = 0, count = v.size(); i < count; ++i)
            add(v[i]);
    }
    
    size_t hash() {
        return static_cast<size_t>(_hasher.hash());
    }
    
private:
    Murmur332Hasher _hasher;
};

}

#endif
