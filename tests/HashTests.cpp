#include "gtest/gtest.h"

#include "MurmurHash3.hpp"

#include "Stle/U/HashU.hpp"

TEST(HashTests, TestMurmur332Hasher) {
    const char *data = "";
    int len = 0;
    uint32_t expected = 0;    
    MurmurHash3_x86_32(data, len, 0, &expected);
    
    U::Murmur332Hasher hasher;
    hasher.addBytes(data, len);    
    ASSERT_EQ(expected, hasher.hash());
    
    data = "abcd";
    len = static_cast<int>(strlen(data));
    MurmurHash3_x86_32(data, len, 0, &expected);
    
    hasher = U::Murmur332Hasher();
    hasher.addBytes(data, len);    
    ASSERT_EQ(expected, hasher.hash());
    
    hasher = U::Murmur332Hasher();
    for (int i = 0; i < len; ++i)
        hasher.addBytes(data+i, 1);
    ASSERT_EQ(expected, hasher.hash());
    
    data = "The quick brown fox jumps over the lazy dog";
    len = static_cast<int>(strlen(data));
    MurmurHash3_x86_32(data, len, 0, &expected);
    
    hasher = U::Murmur332Hasher();
    hasher.addBytes(data, len);    
    ASSERT_EQ(expected, hasher.hash());
    
    hasher = U::Murmur332Hasher();
    for (int i = 0; i < len; ++i)
        hasher.addBytes(data+i, 1);
    ASSERT_EQ(expected, hasher.hash());
}
