#ifndef U_FILE_REF_HPP
#define U_FILE_REF_HPP

#include "Api.hpp"

#include "File.hpp"

namespace U {

class U_API FileRef {
public:
    FileRef() : _file(nullptr) {    
    }
    
    explicit FileRef(const File &file) : _file(file.file()) {        
    }
    
    explicit FileRef(FILE *file) : _file(file) {        
    }
    
    FILE *file() const {
        return _file;
    }
    
private:
    FILE *_file;
};

}

#endif
