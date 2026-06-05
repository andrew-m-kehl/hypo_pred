#ifndef U_FILE_READER_HPP
#define U_FILE_READER_HPP

#include "Api.hpp"

#include "IOError.hpp"
#include "ReaderRef.hpp"
#include "File.hpp"
#include "FileRef.hpp"
#include "OfFile.hpp"

namespace U {
    
// TODO: Buffered file reader.
template <typename TFile>
class FileReader {
public:
    // TODO: Need better api, these tags look bad.
    template<typename... TArgs>
    explicit FileReader(OfFileTag, TArgs&&... args) : _file(std::forward<TArgs>(args)...) {
    }
    
    int getc();
    
    int ungetc(int c);
    
private:
    TFile _file;
};

typedef FileReader<FileRef> FileRefReader;

template<typename TFile>
int FileReader<TFile>::getc() {
    return fgetc(_file.file());
}

template<typename TFile>
int FileReader<TFile>::ungetc(int c) {
    return ::ungetc(c, _file.file());
}

}

#endif
