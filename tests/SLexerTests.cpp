#include "gtest/gtest.h"

#include "Stle/U/SLexer.hpp"
#include "Stle/U/StrReader.hpp"


TEST(SLexerTests, TestReadChar) {
    {
        const char *str0 = "\t \n \r ( ) abc - -. .- .abc -abc";
        U::SLexer<U::StrReader> l(U::OF_READER, str0);
        ASSERT_EQ(l.token(), U::ST_EMPTY);
        ASSERT_EQ(std::string(""), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_LP, l.token());
        ASSERT_EQ(1, l.len());
        ASSERT_EQ(std::string("("), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_RP, l.token());
        ASSERT_EQ(1, l.len());
        ASSERT_EQ(std::string(")"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("abc"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(1, l.len());
        ASSERT_EQ(std::string("-"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string("-."), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string(".-"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string(".abc"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string("-abc"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
        ASSERT_EQ(0, l.len());
        ASSERT_EQ(std::string(""), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    {
        const char *str1 = "0 10 -1 -12";
        U::SLexer<U::StrReader> l(U::OF_READER, str1);
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(1, l.len());
        ASSERT_EQ(std::string("0"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string("10"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string("-1"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("-12"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
      
    {
        const char *str2 = "0.9 0.89 98.76 .5 .54 3. 32.";
        U::SLexer<U::StrReader> l(U::OF_READER, str2);
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("0.9"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string("0.89"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(5, l.len());
        ASSERT_EQ(std::string("98.76"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string(".5"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string(".54"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(2, l.len());
        ASSERT_EQ(std::string("3."), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("32."), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    {
        const char *str3 = "-0.9 -0.89 -98.76 -.5 -.54 -3. -32.";
        U::SLexer<U::StrReader> l(U::OF_READER, str3);
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string("-0.9"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(5, l.len());
        ASSERT_EQ(std::string("-0.89"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(6, l.len());
        ASSERT_EQ(std::string("-98.76"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("-.5"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string("-.54"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("-3."), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(4, l.len());
        ASSERT_EQ(std::string("-32."), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    // Exponent in the number.
    {
        const char *str = "0e0 10E32 -1e-2 -12E-34 -0.9e8 98.76e-54 -3.e4 -32.e10";
        U::SLexer<U::StrReader> l(U::OF_READER, str);
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(3, l.len());
        ASSERT_EQ(std::string("0e0"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(5, l.len());
        ASSERT_EQ(std::string("10E32"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(5, l.len());
        ASSERT_EQ(std::string("-1e-2"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(7, l.len());
        ASSERT_EQ(std::string("-12E-34"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(6, l.len());
        ASSERT_EQ(std::string("-0.9e8"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(9, l.len());
        ASSERT_EQ(std::string("98.76e-54"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(5, l.len());
        ASSERT_EQ(std::string("-3.e4"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(7, l.len());
        ASSERT_EQ(std::string("-32.e10"), l.str());
        
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    {
        const char *str4 = "12a";        
        U::SLexer<U::StrReader> l(U::OF_READER, str4);
        ASSERT_THROW(l.read(), U::SLexerError);
        ASSERT_EQ(U::ST_ERROR, l.token());
    }
    
    {
        const char *str5 = "(max x0 0)";        
        U::SLexer<U::StrReader> l(U::OF_READER, str5);
        l.read();
        ASSERT_EQ(U::ST_LP, l.token());
        ASSERT_EQ(std::string("("), l.str());
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(std::string("max"), l.str());
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(std::string("x0"), l.str());
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(std::string("0"), l.str());
        l.read();
        ASSERT_EQ(U::ST_RP, l.token());
        ASSERT_EQ(std::string(")"), l.str());
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    {
        const char *str6 = "(F (0 0.5) x0)";
        U::SLexer<U::StrReader> l(U::OF_READER, str6);
        l.read();
        ASSERT_EQ(U::ST_LP, l.token());
        ASSERT_EQ(std::string("("), l.str());
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(std::string("F"), l.str());
        l.read();
        ASSERT_EQ(U::ST_LP, l.token());
        ASSERT_EQ(std::string("("), l.str());
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(std::string("0"), l.str());
        l.read();
        ASSERT_EQ(U::ST_NUMBER, l.token());
        ASSERT_EQ(std::string("0.5"), l.str());
        l.read();
        ASSERT_EQ(U::ST_RP, l.token());
        ASSERT_EQ(std::string(")"), l.str());
        l.read();
        ASSERT_EQ(U::ST_SYMBOL, l.token());
        ASSERT_EQ(std::string("x0"), l.str());
        l.read();
        ASSERT_EQ(U::ST_RP, l.token());
        ASSERT_EQ(std::string(")"), l.str());
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
    
    {
        const char *str7 = "\"\" \"abc\" \"a\\\"b\\n\\tc\"";
        U::SLexer<U::StrReader> l(U::OF_READER, str7);
        l.read();
        ASSERT_EQ(U::ST_STRING, l.token());
        ASSERT_EQ(std::string(""), l.str());
        l.read();
        ASSERT_EQ(U::ST_STRING, l.token());
        ASSERT_EQ(std::string("abc"), l.str());
        l.read();
        ASSERT_EQ(U::ST_STRING, l.token());
        ASSERT_EQ(std::string("a\"b\n\tc"), l.str());
        l.read();
        ASSERT_EQ(U::ST_EOF, l.token());
    }
}
