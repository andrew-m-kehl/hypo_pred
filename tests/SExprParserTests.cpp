#include "gtest/gtest.h"

#include "Stle/SExprParser.hpp"

#include "Stle/U/FileU.hpp"

using namespace Stle;

TEST(SExprParserTests, TestParseChar) {
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");    
    ExprPtr x1 = s.makeVar("x1");
    ExprPtr x2 = s.makeVar("x2");
    ExprPtr c0 = s.makeConst(0);
    ExprPtr c1 = s.makeConst(1);
    {    
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "x0 x1");
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(x0, result0);    
        ExprPtr result1 = parser.parse();
        ASSERT_EQ(x1, result1);
    }
    {
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "(max) (max x0) (max x0 x1) (max x0 x1 x2)");
        ExprPtr e0 = s.makeApply(APPLY_MAX, {});
        ExprPtr e1 = s.makeApply(APPLY_MAX, {x0});
        ExprPtr e2 = s.makeApply(APPLY_MAX, {x0, x1});
        ExprPtr e3 = s.makeApply(APPLY_MAX, {x0, x1, x2});
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(e0, result0);
        ExprPtr result1 = parser.parse();
        ASSERT_EQ(e1, result1);
        ExprPtr result2 = parser.parse();
        ASSERT_EQ(e2, result2);
        ExprPtr result3 = parser.parse();
        ASSERT_EQ(e3, result3);
    }
    {
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "(* ) (* x0) (* x0 x1) (* x0 x1 x2)");
        ExprPtr e0 = s.makeApply(APPLY_MUL, {});
        ExprPtr e1 = s.makeApply(APPLY_MUL, {x0});
        ExprPtr e2 = s.makeApply(APPLY_MUL, {x0, x1});
        ExprPtr e3 = s.makeApply(APPLY_MUL, {x0, x1, x2});
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(e0, result0);
        ExprPtr result1 = parser.parse();
        ASSERT_EQ(e1, result1);
        ExprPtr result2 = parser.parse();
        ASSERT_EQ(e2, result2);
        ExprPtr result3 = parser.parse();
        ASSERT_EQ(e3, result3);
    }
    {
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "(/ x0 x1)");
        ExprPtr e0 = s.makeApply(APPLY_DIV, {x0, x1});
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(e0, result0);
    }
    {    
        SExprParser<U::SLexer<U::StrReader>> parser3(s, U::OF_READER, "0");
        ExprPtr result3 = parser3.parse();
        ASSERT_EQ(c0, result3);
    }
    {    
        SExprParser<U::SLexer<U::StrReader>> parser4(s, U::OF_READER, "1.0");
        ExprPtr result4 = parser4.parse();
        ASSERT_EQ(c1, result4);
    }
    {
        ExprPtr e5 = s.makeApply(APPLY_ITE, {x0, x1, c0});
        SExprParser<U::SLexer<U::StrReader>> parser5(s, U::OF_READER, "(ite x0 x1 0)");
        ExprPtr result5 = parser5.parse();
        ASSERT_EQ(e5, result5);
    }
    {
        ExprPtr e6 = s.makeAggregate(AGGREGATE_MIN, x0);
        ExprPtr e7 = s.makeOn(ClosedIntvl(0, 1), e6);
        SExprParser<U::SLexer<U::StrReader>> parser7(s, U::OF_READER, "(On (0 1) (Min x0))");
        ExprPtr result7 = parser7.parse();
        ASSERT_EQ(e7, result7);
    }
    {
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "(Until (0 1) 0 (Min x0) x1) (Until (0 1) 0 (Get x0) x1)");
        ExprPtr e0 = s.makeUntil(ClosedIntvl(0, 1), Space(0), s.makeAggregate(AGGREGATE_MIN, x0), x1);
        ExprPtr e1 = s.makeUntil(ClosedIntvl(0, 1), Space(0), s.makeGet(x0), x1);        
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(e0, result0);
        ExprPtr result1 = parser.parse();
        ASSERT_EQ(e1, result1);
    }
    {        
        SExprParser<U::SLexer<U::StrReader>> parser(s, U::OF_READER, "(F (0 5e-1) (max x0 x1)) (G (0 1E2) (max x0 x1))");
        ExprPtr e0 = s.makeTemporal(ClosedIntvl(0.0, 0.5), TEMPORAL_F, s.makeApply(APPLY_MAX, {x0, x1}));
        ExprPtr e1 = s.makeTemporal(ClosedIntvl(0, 100), TEMPORAL_G, s.makeApply(APPLY_MAX, {x0, x1}));
        ExprPtr result0 = parser.parse();
        ASSERT_EQ(e0, result0);
        ExprPtr result1 = parser.parse();
        ASSERT_EQ(e1, result1); 
    }

    // TODO: Need more tests here for different apply expressions, StlUntil.
}
