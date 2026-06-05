#ifndef U_READER_REF_HPP
#define U_READER_REF_HPP

#include "Api.hpp"

namespace U {

template <typename TReader>
class ReaderRef {
public:
    explicit ReaderRef(TReader &reader) : _reader(&reader) {        
    }
    
    int getc() {
        return _reader->getc();
    }
    
    int ungetc(int c) {
        return _reader->ungetc(c);
    }
    
private:
    TReader *_reader;
};

}

#endif
