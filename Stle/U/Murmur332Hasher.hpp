#ifndef U_MURMUR3_32_HASHER_HPP
#define U_MURMUR3_32_HASHER_HPP

#include "Api.hpp"

#include <cstdint>

namespace U {

class U_API Murmur332Hasher {
public:
    Murmur332Hasher(uint32_t seed = 0)
        : _h1(seed), _buffer {0,0,0,0}, _bufferLen(0), _totalLen(0) {        
    }
    
    void addBytes(const char *data, int len);
 
private:
    inline void addBlock(uint32_t k1);
    
public:
    uint32_t hash();    

private:    
    uint32_t _h1;
    char _buffer[4];
    int _bufferLen;
    int _totalLen;
};

}

#endif
