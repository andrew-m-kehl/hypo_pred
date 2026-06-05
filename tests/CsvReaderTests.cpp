#include "gtest/gtest.h"

#include "Stle/U/StrReader.hpp"
#include "Stle/U/CsvReader.hpp"

TEST(CsvReaderTests, TestReadChar) {
    {
        const char *csv = "0.1,0.2,0.3\n,0.5,";
                
        U::StrReader charReader(csv);
        U::CsvReader<U::ReaderRef<U::StrReader>> r(U::OF_READER, charReader);
        
        ASSERT_EQ(U::CSV_NONE, r.token());
        ASSERT_EQ(0, r.len());
        ASSERT_EQ(std::string(""), r.str());
    
        U::CsvToken token = U::CSV_NONE;
                
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(3, r.len());
        ASSERT_EQ(std::string("0.1"), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(3, r.len());
        ASSERT_EQ(std::string("0.2"), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(3, r.len());
        ASSERT_EQ(std::string("0.3"), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_LINE_END, token);
        ASSERT_EQ(0, r.len());
        ASSERT_EQ(std::string(""), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(0, r.len());
        ASSERT_EQ(std::string(""), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(3, r.len());
        ASSERT_EQ(std::string("0.5"), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_FIELD, token);
        ASSERT_EQ(0, r.len());
        ASSERT_EQ(std::string(""), r.str());
        
        token = r.read();
        ASSERT_EQ(U::CSV_EOF, token);
        ASSERT_EQ(0, r.len());
        ASSERT_EQ(std::string(""), r.str());
    }
}
