#ifndef F_OPEN_HPP
#define F_OPEN_HPP

#include "Api.hpp"

#include <cstdio>

namespace U {

    U_API FILE *u8FOpen(const char *u8Name, const char *mode, bool nothrow = true);
}


#endif
