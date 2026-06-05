#ifndef U_ARRAY_2D_REF_HPP
#define U_ARRAY_2D_REF_HPP

#include "Api.hpp"

#include <stdexcept>

namespace U {

template <typename T>
class Array2DRef{ 
public:
    Array2DRef(T *data, int width) : _data(data), _width(width) {
#ifndef NDEBUG
        if (width <= 0)
            throw std::runtime_error("Width should be positive.");
#endif
    }
    
    T *operator[](int row) const {
        return _data + (row * _width);
    }
    
private:
    T *_data;
    int _width;
};

template <typename T>
Array2DRef<T> makeArray2DRef(T *data, int width) {
    return Array2DRef<T>(data, width);
}

}

#endif
