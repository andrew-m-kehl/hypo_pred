#include "Murmur332Hasher.hpp"

#if defined(_MSC_VER)
#include <cstdlib>
#define ROTL32(x,y) _rotl(x,y)
#else
inline static uint32_t rotl32 ( uint32_t x, int8_t r ) {
  return (x << r) | (x >> (32 - r));
}
#define	ROTL32(x,y) rotl32(x,y)
#endif

static constexpr uint32_t c1 = 0xcc9e2d51;
static constexpr uint32_t c2 = 0x1b873593;

namespace U {

void Murmur332Hasher::addBytes(const char *data, int len) {
    int shift = 0;
    if (_bufferLen != 0) {
        shift = (len < 4 - _bufferLen) ? len : 4 - _bufferLen;
        for (int i = 0; i < shift; ++i)
            _buffer[_bufferLen + i] = data[i];            
        _bufferLen += shift;
        if (_bufferLen == 4) {
            uint32_t *buffer32 = reinterpret_cast<uint32_t*>(_buffer);
            addBlock(*buffer32);
            _bufferLen = 0;
            *buffer32 = 0;
        }            
    }
    
    const int nblocks = (len - shift) / 4;
    const uint32_t *blocks = reinterpret_cast<const uint32_t*>(data + shift + nblocks*4);
    for(int i = -nblocks; i; i++) {
        addBlock(blocks[i]);
    }
    
    const char *tail = reinterpret_cast<const char*>(data + shift + nblocks*4);
    int tailLen = len - shift - nblocks*4;
    if (tailLen != 0) {
        for (int i=0; i < tailLen; ++i) {
            _buffer[i] = tail[i];
        }
        _bufferLen = tailLen;
    }
    
    _totalLen += len;
}

void Murmur332Hasher::addBlock(uint32_t k1) {
    k1 *= c1;
    k1 = ROTL32(k1,15);
    k1 *= c2;
    
    _h1 ^= k1;
    _h1 = ROTL32(_h1,13); 
    _h1 = _h1*5+0xe6546b64;
}

uint32_t Murmur332Hasher::hash() {
    uint32_t k1 = 0;
    uint32_t h1 = _h1;
    
    switch(_bufferLen) {
    case 3: k1 ^= _buffer[2] << 16;
            // fall through
    case 2: k1 ^= _buffer[1] << 8;
            // fall through
    case 1: k1 ^= _buffer[0];
        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;
        h1 ^= k1;
    };
    
    h1 ^= _totalLen;
    
    // fmix32
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    
    return h1;
}

}
