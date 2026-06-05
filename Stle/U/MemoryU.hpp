#ifndef U_MEMORY_U_HPP
#define U_MEMORY_U_HPP

#include "Api.hpp"

namespace U {

class U_API CannotCopy {
public:
    constexpr CannotCopy() = default;
    ~CannotCopy() = default;

    CannotCopy(CannotCopy&&) = default;
    CannotCopy& operator=(CannotCopy&&) = default;

    CannotCopy(const CannotCopy&) = delete;
    CannotCopy& operator=(const CannotCopy&) = delete;
};

}

#endif
