#ifndef U_FILE_HPP
#define U_FILE_HPP

#include "Api.hpp"

#include "FOpen.hpp"

#include "MemoryU.hpp"

#include <stdexcept>

namespace U {

class U_API File : CannotCopy {
public:
    File() : _file(nullptr) {        
    }
    
    File(const char *u8Name, const char *mode);
    
    ~File();
    
    FILE *file() const {
        return _file;
    }    
    
    File(File &&other) noexcept;
    
    File& operator=(File&& other);
    
    long size();
    
    int error();
    
private:
    FILE *_file;
};

}

#endif
