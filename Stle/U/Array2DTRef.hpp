#ifndef U_ARRAY_2D_T_REF_HPP
#define U_ARRAY_2D_T_REF_HPP

#include "Api.hpp"

namespace U {

template<typename T>
class RowTRef {
public:
    RowTRef() : _data(nullptr), _height(0) {        
    }
     
    RowTRef(T *data, int height) : _data(data), _height(height) {
#ifndef NDEBUG
        if (height <= 0)
            throw std::runtime_error("Height should be positive.");
#endif
    }
    
    T operator[](int column) const {
        return *(_data + column * _height);
    }
    
    T *data() const {
        return _data;
    }
    
    int height() const {
        return _height;
    }
    
private:
    T *_data;
    int _height;
};

template<typename T>
class Array2DTRef {    
public:
    Array2DTRef(T *data, int height) : _data(data), _height(height) {
#ifndef NDEBUG
        if (height <= 0)
            throw std::runtime_error("Height should be positive.");
#endif
    }
    
    RowTRef<T> operator[](int row) const {
        return RowTRef<T>(_data + row, _height);
    }
    
    T *data() const {
        return _data;
    }
    
    int height() const {
        return _height;
    }
    
private:
    T *_data;
    int _height;
};

template <typename T>
Array2DTRef<T> makeArray2DTRef(T *data, int height) {
    return Array2DTRef<T>(data, height);
}

}

#endif
