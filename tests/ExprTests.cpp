#include "gtest/gtest.h"

#include "Stle/U/StrU.hpp"

#include "Stle/Expr.hpp"
#include "Stle/ExprSet.hpp"
#include "Stle/SExprParser.hpp"
#include "Stle/PCRewriter.hpp"

#include "Stle/U/FilePrinter.hpp"

using namespace Stle;

TEST(ExprTests, TestCreateToString) {
    {
        // Creating with ExprSet.
        ExprSet s;
        
        ExprPtr x0 = s.makeVar("x0");    
        ASSERT_EQ("x0", U::toString(*x0));
        
        ExprPtr x1 = s.makeVar("x1"); 
        ExprPtr max01 = s.makeApply(APPLY_MAX, {x0, x1});
        ASSERT_EQ("(max x0 x1)", U::toString(max01));
                
        ExprPtr c0 = s.makeConst(0);
        ASSERT_EQ("0", U::toString(*c0));
        
        ExprPtr b0 = s.makeBoolConst(false);
        ASSERT_EQ("false", U::toString(b0));
        
        ExprPtr b1 = s.makeBoolConst(true);
        ASSERT_EQ("true", U::toString(b1));
        
        ExprPtr minC0X0 = s.makeApply(APPLY_MIN, {c0, x0});
        ASSERT_EQ("(min 0 x0)", U::toString(minC0X0));
        
        ExprPtr iteX0X1C0 = s.makeApply(APPLY_ITE, {x0, x1, c0});
        ASSERT_EQ("(ite x0 x1 0)", U::toString(iteX0X1C0));
        
        ExprPtr minX0 = s.makeAggregate(AGGREGATE_MIN, x0);
        ASSERT_EQ("(Min x0)", U::toString(minX0));
        
        ExprPtr on01MinX0 = s.makeOn(ClosedIntvl(0, 1), minX0);
        ASSERT_EQ("(On (0 1) (Min x0))", U::toString(on01MinX0));
        
        ExprPtr until1 = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX0, x1);
        ASSERT_EQ("(Until (0 1) 0 (Min x0) x1)", U::toString(until1));
        
        ExprPtr get1 = s.makeGet(x0);
        ASSERT_EQ("(Get x0)", U::toString(*get1));
        
        ExprPtr until3 = s.makeUntil(ClosedIntvl(0, 1), Space(0), get1, x1);
        ASSERT_EQ("(Until (0 1) 0 (Get x0) x1)", U::toString(until3));
        
        ExprPtr delay10X0 = s.makeLookup(Time(1), Space(0), x0);
        ASSERT_EQ("(Lookup 1 0 x0)", U::toString(delay10X0));
        
        ExprPtr f10X0 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, x0);
        ASSERT_EQ("(F (0 1) x0)", U::toString(f10X0));
        
        ExprPtr tempUntil1 = s.makeTemporalUntil(ClosedIntvl(0, 1), x0, x1);
        ASSERT_EQ("(StlUntil (0 1) x0 x1)", U::toString(tempUntil1));
    }    
}

TEST(ExprTests, TestExprSet) {
    ExprSet s;
    
    ExprPtr x0 = s.makeVar("x0");
    ExprPtr x0Copy = s.makeVar("x0");
    ExprPtr x1 = s.makeVar("x1");
    ASSERT_EQ(x0, x0Copy);    
    ASSERT_NE(x0, x1);
    
    ExprPtr c0 = s.makeConst(0);
    ExprPtr c0Copy = s.makeConst(0);
    ExprPtr c1 = s.makeConst(1);
    ASSERT_EQ(c0, c0Copy);
    ASSERT_NE(c0, c1);
    
    ExprPtr cInf = s.makeConst(Space::inf());
    ExprPtr cInfCopy = s.makeConst(Space::inf());
    ASSERT_EQ(cInf, cInfCopy);
    
    ExprPtr b0 = s.makeBoolConst(false);
    ExprPtr b0Copy = s.makeBoolConst(false);
    ExprPtr b1 = s.makeBoolConst(true);
    ASSERT_EQ(b0, b0Copy);
    ASSERT_NE(b0, b1);
    
    ExprPtr minC0X0 = s.makeApply(APPLY_MIN, {c0, x0});
    ExprPtr minC0X0Copy = s.makeApply(APPLY_MIN, {c0, x0});
    ExprPtr minC0X1 = s.makeApply(APPLY_MIN, {c0, x1});
    ExprPtr minC1X0 = s.makeApply(APPLY_MIN, {c1, x0});
    ExprPtr maxC0X0 = s.makeApply(APPLY_MAX, {c0, x0});
    ASSERT_EQ(minC0X0, minC0X0Copy);
    ASSERT_NE(minC0X0, minC0X1);
    ASSERT_NE(minC0X0, minC1X0);
    ASSERT_NE(minC0X0, maxC0X0);
    
    ExprPtr minX0 = s.makeAggregate(AGGREGATE_MIN, x0);
    ExprPtr minX0Copy = s.makeAggregate(AGGREGATE_MIN, x0);
    ExprPtr minX1 = s.makeAggregate(AGGREGATE_MIN, x1);
    ExprPtr maxX0 = s.makeAggregate(AGGREGATE_MAX, x0);
    ASSERT_EQ(minX0, minX0Copy);
    ASSERT_NE(minX0, minX1);
    ASSERT_NE(minX0, maxX0);
    
    ExprPtr on01MinX0 = s.makeOn(ClosedIntvl(0, 1), minX0);
    ExprPtr on01MinX0Copy = s.makeOn(ClosedIntvl(0, 1), minX0);
    ExprPtr on02MinX0 = s.makeOn(ClosedIntvl(0, 2), minX0);
    ExprPtr on01MinX1 = s.makeOn(ClosedIntvl(0, 1), minX1);
    ASSERT_EQ(on01MinX0, on01MinX0Copy);
    ASSERT_NE(on01MinX0, on02MinX0);
    ASSERT_NE(on01MinX0, on01MinX1);
    
    ExprPtr until1 = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX0, x1);
    ExprPtr until1Copy = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX0, x1);    
    ASSERT_EQ(until1, until1Copy);
    ExprPtr until2 = s.makeUntil(ClosedIntvl(0, 1), Space(1), minX0, x1);
    ASSERT_NE(until1, until2);
    until2 = s.makeUntil(ClosedIntvl(1, 2), Space(0), minX0, x1);
    ASSERT_NE(until1, until2);
    until2 = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX1, x1);
    ASSERT_NE(until1, until2);
    until2 = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX0, x0);
    ASSERT_NE(until1, until2);
    
    ExprPtr get1 = s.makeGet(x0);
    ExprPtr get1Copy = s.makeGet(x0);
    ASSERT_EQ(get1, get1Copy);
    ExprPtr get2 = s.makeGet(x1);
    ASSERT_NE(get1, get2);
    
    ExprPtr delay10X0 = s.makeLookup(Time(1), Space(0), x0);
    ExprPtr delay10X0Copy = s.makeLookup(Time(1), Space(0), x0);
    ASSERT_EQ(delay10X0, delay10X0Copy);
    ExprPtr delay2 = s.makeLookup(Time(2), Space(0), x0);
    ASSERT_NE(delay10X0, delay2);
    delay2 = s.makeLookup(Time(1), Space(1), x0);
    ASSERT_NE(delay10X0, delay2);
    delay2 = s.makeLookup(Time(1), Space(0), x1);
    ASSERT_NE(delay10X0, delay2);
    
    ExprPtr f10X0 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, x0);
    ExprPtr f10X0Copy = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, x0);
    ASSERT_EQ(f10X0, f10X0Copy);
    ExprPtr f2 = s.makeTemporal(ClosedIntvl(0, 2), TEMPORAL_F, x0);
    ASSERT_NE(f10X0, f2);
    f2 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_G, x0);
    ASSERT_NE(f10X0, f2);
    f2 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, x1);    
    ASSERT_NE(f10X0, f2);
    
    ExprPtr tempUntil1 = s.makeTemporalUntil(ClosedIntvl(0, 1), x0, x1);
    ExprPtr tempUntil1Copy = s.makeTemporalUntil(ClosedIntvl(0, 1), x0, x1);
    ASSERT_EQ(tempUntil1, tempUntil1Copy);
    ExprPtr tempUntil2 = s.makeTemporalUntil(ClosedIntvl(1, 2), x0, x1);
    ASSERT_NE(tempUntil1, tempUntil2);
    tempUntil2 = s.makeTemporalUntil(ClosedIntvl(0, 1), x1, x1);
    ASSERT_NE(tempUntil1, tempUntil2);
    tempUntil2 = s.makeTemporalUntil(ClosedIntvl(0, 1), x0, x0);
    ASSERT_NE(tempUntil1, tempUntil2);
}

TEST(ExprTests, TextExprKindMeet) {
    ASSERT_EQ(EXPR_KIND_BOTTOM, exprKindMeet(EXPR_KIND_BOTTOM, EXPR_KIND_BOTTOM));
    ASSERT_EQ(EXPR_KIND_POINT, exprKindMeet(EXPR_KIND_POINT, EXPR_KIND_POINT));
    ASSERT_EQ(EXPR_KIND_BOTTOM, exprKindMeet(EXPR_KIND_POINT, EXPR_KIND_INTVL));
    ASSERT_EQ(EXPR_KIND_BOTTOM, exprKindMeet(EXPR_KIND_INTVL, EXPR_KIND_POINT));
    ASSERT_EQ(EXPR_KIND_BOTTOM, exprKindMeet(EXPR_KIND_POINT, EXPR_KIND_UNTIL));
    ASSERT_EQ(EXPR_KIND_BOTTOM, exprKindMeet(EXPR_KIND_UNTIL, EXPR_KIND_POINT));
    ASSERT_EQ(EXPR_KIND_INTVL, exprKindMeet(EXPR_KIND_INTVL, EXPR_KIND_INTVL));
    ASSERT_EQ(EXPR_KIND_UNTIL, exprKindMeet(EXPR_KIND_INTVL, EXPR_KIND_UNTIL));
    ASSERT_EQ(EXPR_KIND_UNTIL, exprKindMeet(EXPR_KIND_UNTIL, EXPR_KIND_INTVL));
    ASSERT_EQ(EXPR_KIND_UNTIL, exprKindMeet(EXPR_KIND_UNTIL, EXPR_KIND_UNTIL));
}

TEST(ExprTests, TextExprTypeKind) {
    ExprSet s;
    
    ExprPtr x0 = s.makeVar("x0");
    ASSERT_EQ(EXPR_VAR, x0->type());
    ASSERT_EQ(EXPR_KIND_POINT, x0->kind());
    
    ExprPtr c0 = s.makeConst(0);
    ASSERT_EQ(EXPR_CONST, c0->type());
    ASSERT_EQ(EXPR_KIND_POINT, c0->kind());
    
    ExprPtr b0 = s.makeBoolConst(false);
    ASSERT_EQ(EXPR_BOOL_CONST, b0->type());
    ASSERT_EQ(EXPR_KIND_POINT, b0->kind());
    
    ExprPtr minC0X0 = s.makeApply(APPLY_MIN, {c0, x0});
    ASSERT_EQ(EXPR_APPLY, minC0X0->type());
    ASSERT_EQ(EXPR_KIND_POINT, minC0X0->kind());
    
    ExprPtr minX0 = s.makeAggregate(AGGREGATE_MIN, x0);
    ASSERT_EQ(EXPR_AGGREGATE, minX0->type());
    ASSERT_EQ(EXPR_KIND_INTVL, minX0->kind());
    
    ExprPtr x1 = s.makeVar("x1");    
    ExprPtr minX1 = s.makeAggregate(AGGREGATE_MIN, x1);
    ExprPtr maxMinX0MinX1 = s.makeApply(APPLY_MAX, {minX0, minX1});
    ASSERT_EQ(EXPR_APPLY, maxMinX0MinX1->type());
    ASSERT_EQ(EXPR_KIND_INTVL, maxMinX0MinX1->kind());
    
    ASSERT_THROW(s.makeApply(APPLY_MAX, {minX0, x0}), ExprError);
    
    ExprPtr on01MinX0 = s.makeOn(ClosedIntvl(0, 1), minX0);
    ASSERT_EQ(EXPR_ON, on01MinX0->type());
    ASSERT_EQ(EXPR_KIND_POINT, on01MinX0->kind());
    
    ExprPtr until1 = s.makeUntil(ClosedIntvl(0, 1), Space(0), minX0, x1);
    ASSERT_EQ(EXPR_UNTIL, until1->type());
    ASSERT_EQ(EXPR_KIND_POINT, until1->kind());
    
    ExprPtr getX0 = s.makeGet(x0);
    ASSERT_EQ(EXPR_GET, getX0->type());
    ASSERT_EQ(EXPR_KIND_UNTIL, getX0->kind());
    
    ASSERT_THROW(s.makeApply(APPLY_MIN, {getX0, x0}), ExprError);
    
    ExprPtr minGetX0MinX1 = s.makeApply(APPLY_MIN, {getX0, minX1});
    ASSERT_EQ(EXPR_APPLY, minGetX0MinX1->type());
    ASSERT_EQ(EXPR_KIND_UNTIL, minGetX0MinX1->kind());
    
    ExprPtr delay10X0 = s.makeLookup(Time(1), Space(0), x0);
    ASSERT_EQ(EXPR_LOOKUP, delay10X0->type());
    ASSERT_EQ(EXPR_KIND_POINT, delay10X0->kind());
    
    ExprPtr f10X0 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, x0);
    ASSERT_EQ(EXPR_TEMPORAL, f10X0->type());
    ASSERT_EQ(EXPR_KIND_POINT, f10X0->kind()); 
    
    ExprPtr tempUntil1 = s.makeTemporalUntil(ClosedIntvl(0, 1), x0, x1);
    ASSERT_EQ(EXPR_TEMPORAL_UNTIL, tempUntil1->type());
    ASSERT_EQ(EXPR_KIND_POINT, tempUntil1->kind());
}
