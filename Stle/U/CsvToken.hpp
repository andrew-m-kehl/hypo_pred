#ifndef U_CSV_TOKEN_HPP
#define U_CSV_TOKEN_HPP

#include "Api.hpp"

namespace U {

enum CsvToken {
    CSV_NONE,
    CSV_FIELD,
    CSV_LINE_END,
    CSV_EOF    
};

}

#endif
