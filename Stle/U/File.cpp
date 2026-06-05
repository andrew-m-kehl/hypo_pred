#include "File.hpp"

#include "IOError.hpp"

namespace U {

File::File(const char *u8Name, const char *mode) : _file(nullptr) {
    _file = u8FOpen(u8Name, mode, false);    
}

File::~File() {
    if (_file) {
        fclose(_file);
        _file = nullptr;
    }    
}

File::File(File &&other) noexcept : _file(other._file) {
    other._file = nullptr;       
}

File &File::operator=(File &&other) {
    if (&other == this)
        return *this;

    if(_file)
        fclose(_file);
    _file = other._file;
    other._file = nullptr;
    return *this;
}

// TODO: Need tests for Files.
long File::size() {
    long pos = ftell(_file);
    if (pos == -1)
        throw IOError("ftell error.");
    int error = fseek(_file, 0, SEEK_END);
    if (error)
        throw IOError("fseek error.");
    long result = ftell(_file);
    if (result == -1)
        throw IOError("ftell error.");
    error = fseek(_file, pos, SEEK_SET);
    if (error)
        throw IOError("fseek error.");
    return result;        
}

int File::error() {
    return ferror(_file);
}


}
