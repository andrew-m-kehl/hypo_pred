#include "gtest/gtest.h"

#include "Stle/U/StrU.hpp"
#include "Stle/U/VPrinter.hpp"
#include "Stle/U/VPrinterRef.hpp"

TEST(StrPrinterTests, TestStrPrinter) {
    {
        U::StrPrinter sp;
        
        sp.puts("abc");
        ASSERT_EQ("abc", std::string(sp.str()));
        
        sp.printf("%d%d%d", 1, 2, 3);
        ASSERT_EQ("abc123", std::string(sp.str()));
        
        sp.puts("");
        ASSERT_EQ("abc123", std::string(sp.str()));
        
        sp.puts(std::string("def"));
        ASSERT_EQ("abc123def", std::string(sp.str()));
    }
    {
        U::StrPrinter spOrig;
        U::VPrinterRef<U::StrPrinter> sp(spOrig);
        
        sp.puts("abc");
        ASSERT_EQ("abc", std::string(spOrig.str()));
        
        sp.printf("%d%d%d", 1, 2, 3);
        ASSERT_EQ("abc123", std::string(spOrig.str()));
        
        sp.puts("");
        ASSERT_EQ("abc123", std::string(spOrig.str()));
        
        sp.puts(std::string("def"));
        ASSERT_EQ("abc123def", std::string(spOrig.str()));
    }    
    {
        U::StrPrinter sp;
        sp << "abc" << 1 << 2 << 3 << "" << std::string("def");
        ASSERT_EQ("abc123def", std::string(sp.str()));
    }
}
