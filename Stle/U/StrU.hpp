#ifndef U_STR_U_HPP
#define U_STR_U_HPP

#include "Api.hpp"

#include "StrPrinter.hpp"

#include <string>

namespace U {

template<typename T>
std::string toString(const T &v) {
    U::StrPrinter p;
    p << v;
    return std::string(p.str());            
}

}

#endif
