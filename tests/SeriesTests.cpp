#include "gtest/gtest.h"

#include "Stle/U/StrU.hpp"

#include "Stle/PCSeries.hpp"
#include "Stle/PCMonitorVisitor.hpp"

using namespace Stle;

TEST(SeriesTests, TestCreateToString) {
    PCSeries empty;
    ASSERT_EQ("empty", U::toString(empty));
    
    PCSeries s0(0, true, 0);
    ASSERT_EQ("[0] -> 0", U::toString(s0));
    
    PCSeries s1Open(1, false, 1);
    ASSERT_EQ("[0, 1) -> 1", U::toString(s1Open));
    
    PCSeries s1Closed(1, true, 1);
    ASSERT_EQ("[0, 1) -> 1; [1] -> 1", U::toString(s1Closed));
}

TEST(SeriesTests, TestAddToString) {
    PCSeries s0;
    s0.add(0, 0);
    ASSERT_EQ("[0] -> 0", U::toString(s0));
    
    PCSeries s1;
    s1.add(0, 0);
    s1.add(1, 1);
    ASSERT_EQ("[0] -> 0; (0, 1) -> 1", U::toString(s1));    
    
    PCSeries s1_1;    
    s1_1.add(1, 1);
    ASSERT_EQ("[0, 1) -> 1", U::toString(s1_1));
    
    PCSeries s1_2;    
    s1_2.add(1, 1);
    s1_2.add(1, 1);
    ASSERT_EQ("[0, 1) -> 1; [1] -> 1", U::toString(s1_2));
    
    PCSeries s2_1;
    s2_1.add(0, 0);
    s2_1.add(1, 1);
    s2_1.add(1, 1);
    ASSERT_EQ("[0] -> 0; (0, 1) -> 1; [1] -> 1", U::toString(s2_1));
    
    PCSeries s2_2;
    s2_2.add(0, 0);
    s2_2.add(1, 1);
    s2_2.add(1, 2);
    ASSERT_EQ("[0] -> 0; (0, 1) -> 1; [1] -> 2", U::toString(s2_2));
    
    PCSeries s3;
    s3.add(0, 0);
    s3.add(1, 1);
    s3.add(2, 1);
    ASSERT_EQ("[0] -> 0; (0, 2) -> 1", U::toString(s3));
    
    PCSeries s4;
    s4.add(0, 0);
    s4.add(1, 1);
    s4.add(1, 1);
    s4.add(2, 1);
    ASSERT_EQ("[0] -> 0; (0, 2) -> 1", U::toString(s4));
    
    PCSeries s5;
    s5.add(0, 0);
    s5.add(1, 1);
    s5.add(1, 4);
    s5.add(2, 6);
    ASSERT_EQ("[0] -> 0; (0, 1) -> 1; [1] -> 4; (1, 2) -> 6", U::toString(s5));    
}

Space neg(Space v) {
    return -v;
}

struct Neg {
    Space operator()(Space v) {
        return neg(v);
    }
};

Space zero1(Space) {
    return Space(0);
}

Space zero2(Space, Space) {
    return Space(0);
}

template<typename TIt>
Space zeroR(TIt, TIt) {
    return Space(0);
}

struct Zero1 {
    Space operator()(Space v) {
        return zero1(v);
    }
};

struct Zero2 {
    Space operator()(Space v1, Space v2) {
        return zero2(v1, v2);
    }
};

struct ZeroR {
   template<typename TIt>
    Space operator()(TIt begin, TIt end) {
        return  zeroR(begin, end);
    }
};

TEST(SeriesTests, TestApply1) {
    {
        // Applying Zero1{} that always returns 0.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::apply1(Zero1{}, empty);
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        PCSeries expectedS0;
        expectedS0.add(0, 0);
        PCSeries resultS0 = PCSeries::apply1(Zero1{}, s0);
        ASSERT_EQ(expectedS0, resultS0);
        
        PCSeries s1;
        s1.add(0, 0);
        s1.add(1, 1);
        s1.add(1, 2);
        PCSeries expectedS1;
        expectedS1.add(1, 0);
        expectedS1.add(1, 0);
        PCSeries resultS1 = PCSeries::apply1(Zero1{}, s1);
        ASSERT_EQ(expectedS1, resultS1);
    }
    {
        // Applying neg that negates the result.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::apply1(neg, empty);
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        PCSeries expectedS0;
        expectedS0.add(0, -1);
        PCSeries resultS0 = PCSeries::apply1(neg, s0);
        ASSERT_EQ(expectedS0, resultS0);
        
        PCSeries s1;
        s1.add(0, 0);
        s1.add(1, 1);
        s1.add(1, 2);
        PCSeries expectedS1;
        expectedS1.add(0, 0);
        expectedS1.add(1, -1);
        expectedS1.add(1, -2);
        PCSeries resultS1 = PCSeries::apply1(neg, s1);
        ASSERT_EQ(expectedS1, resultS1);
    }
    
}

TEST(SeriesTests, TestApply2) {
    {
        // Applying Zero2{} that always returns 0.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::apply2(Zero2{}, empty, empty);
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        
        PCSeries s1;
        s1.add(0, 2);
        
        PCSeries expectedS0S1;
        expectedS0S1.add(0, 0);
        
        PCSeries resultS0S1 = PCSeries::apply2(Zero2{}, s0, s1);
        ASSERT_EQ(expectedS0S1, resultS0S1);
        
        PCSeries s2;
        s2.add(1, 1);
        s2.add(1, 2);
        s2.add(2, 3);
        
        PCSeries s3;
        s3.add(0, 0);
        s3.add(1, 1);
        s3.add(2, 2);
        
        PCSeries expectedS2S3;
        expectedS2S3.add(2, 0);
                
        PCSeries resultS2S3 = PCSeries::apply2(Zero2{}, s2, s3);
        ASSERT_EQ(expectedS2S3, resultS2S3);
        
        PCSeries s4;
        s4.add(2, 0);
        s4.add(4, 1);
        s4.add(4, 2);
        s4.add(6, 0);
        s4.add(6, 1);
        
        PCSeries s5;
        s5.add(0, 1);
        s5.add(1, 2);
        s5.add(3, 3);
        s5.add(3, 0);
        s5.add(5, 4);
        s5.add(6, 5);
        s5.add(6, 5);
        
        PCSeries expectedS4S5;
        expectedS4S5.add(6, 0);
        expectedS4S5.add(6, 0);
        
        PCSeries resultS4S5 = PCSeries::apply2(Zero2{}, s4, s5);
        ASSERT_EQ(expectedS4S5, resultS4S5);
        
    }
    {
        // Applying Zero1{} that always returns 0.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::apply2(SpaceMul{}, empty, empty);
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        
        PCSeries s1;
        s1.add(0, 2);
        
        PCSeries expectedS0S1;
        expectedS0S1.add(0, 2);
        
        PCSeries resultS0S1 = PCSeries::apply2(SpaceMul{}, s0, s1);
        ASSERT_EQ(expectedS0S1, resultS0S1);
        
        PCSeries s2;
        s2.add(1, 1);
        s2.add(1, 2);
        s2.add(2, 3);
        
        PCSeries s3;
        s3.add(0, 0);
        s3.add(1, 1);
        s3.add(2, 2);
        
        PCSeries expectedS2S3;
        expectedS2S3.add(0, 0);
        expectedS2S3.add(1, 1);
        expectedS2S3.add(1, 4);
        expectedS2S3.add(2, 6);
        
        PCSeries resultS2S3 = PCSeries::apply2(SpaceMul{}, s2, s3);
        ASSERT_EQ(expectedS2S3, resultS2S3);
        
        PCSeries s4;
        s4.add(2, 0);
        s4.add(4, 1);
        s4.add(4, 2);
        s4.add(6, 0);
        s4.add(6, 1);
        
        PCSeries s5;
        s5.add(0, 1);
        s5.add(1, 2);
        s5.add(3, 3);
        s5.add(3, 0);
        s5.add(5, 4);
        s5.add(6, 5);
        s5.add(6, 5);
        
        PCSeries expectedS4S5;
        expectedS4S5.add(2, 0);
        expectedS4S5.add(3, 3);
        expectedS4S5.add(3, 0);
        expectedS4S5.add(4, 4);
        expectedS4S5.add(4, 8);
        expectedS4S5.add(6, 0);
        expectedS4S5.add(6, 5);
        
        PCSeries resultS4S5 = PCSeries::apply2(SpaceMul{}, s4, s5);
        ASSERT_EQ(expectedS4S5, resultS4S5);        
    }
}

TEST(SeriesTests, TestApplyVararg) {
    {
        // Applying Zero2{} that always returns 0.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::applyI(ZeroR{}, {&empty, &empty});
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        
        PCSeries s1;
        s1.add(0, 2);
        
        PCSeries expectedS0S1;
        expectedS0S1.add(0, 0);
        
        PCSeries resultS0S1 = PCSeries::applyI(ZeroR{}, {&s0, &s1});
        ASSERT_EQ(expectedS0S1, resultS0S1);
        
        PCSeries s2;
        s2.add(1, 1);
        s2.add(1, 2);
        s2.add(2, 3);
        
        PCSeries s3;
        s3.add(0, 0);
        s3.add(1, 1);
        s3.add(2, 2);
        
        PCSeries expectedS2S3;
        expectedS2S3.add(2, 0);
                
        PCSeries resultS2S3 = PCSeries::applyI(ZeroR{}, {&s2, &s3});
        ASSERT_EQ(expectedS2S3, resultS2S3);
        
        PCSeries s4;
        s4.add(2, 0);
        s4.add(4, 1);
        s4.add(4, 2);
        s4.add(6, 0);
        s4.add(6, 1);
        
        PCSeries s5;
        s5.add(0, 1);
        s5.add(1, 2);
        s5.add(3, 3);
        s5.add(3, 0);
        s5.add(5, 4);
        s5.add(6, 5);
        s5.add(6, 5);
        
        PCSeries expectedS4S5;
        expectedS4S5.add(6, 0);
        expectedS4S5.add(6, 0);
        
        PCSeries resultS4S5 = PCSeries::applyI(ZeroR{}, {&s4, &s5});
        ASSERT_EQ(expectedS4S5, resultS4S5);
        
    }
    {
        // Applying zeroVararg that always returns 0.
        
        PCSeries empty;
        PCSeries expectedEmpty;
        PCSeries resultEmpty = PCSeries::applyI(SpaceMulR{}, {&empty, &empty});
        ASSERT_EQ(expectedEmpty, resultEmpty);
        
        PCSeries s0;
        s0.add(0, 1);
        
        PCSeries s1;
        s1.add(0, 2);
        
        PCSeries expectedS0S1;
        expectedS0S1.add(0, 2);
        
        PCSeries resultS0S1 = PCSeries::applyI(SpaceMulR{}, {&s0, &s1});
        ASSERT_EQ(expectedS0S1, resultS0S1);
        
        PCSeries s2;
        s2.add(1, 1);
        s2.add(1, 2);
        s2.add(2, 3);
        
        PCSeries s3;
        s3.add(0, 0);
        s3.add(1, 1);
        s3.add(2, 2);
        
        PCSeries expectedS2S3;
        expectedS2S3.add(0, 0);
        expectedS2S3.add(1, 1);
        expectedS2S3.add(1, 4);
        expectedS2S3.add(2, 6);
        
        PCSeries resultS2S3 = PCSeries::applyI(SpaceMulR{}, {&s2, &s3});
        ASSERT_EQ(expectedS2S3, resultS2S3);
        
        PCSeries s4;
        s4.add(2, 0);
        s4.add(4, 1);
        s4.add(4, 2);
        s4.add(6, 0);
        s4.add(6, 1);
        
        PCSeries s5;
        s5.add(0, 1);
        s5.add(1, 2);
        s5.add(3, 3);
        s5.add(3, 0);
        s5.add(5, 4);
        s5.add(6, 5);
        s5.add(6, 5);
        
        PCSeries expectedS4S5;
        expectedS4S5.add(2, 0);
        expectedS4S5.add(3, 3);
        expectedS4S5.add(3, 0);
        expectedS4S5.add(4, 4);
        expectedS4S5.add(4, 8);
        expectedS4S5.add(6, 0);
        expectedS4S5.add(6, 5);
        
        PCSeries resultS4S5 = PCSeries::applyI(SpaceMulR{}, {&s4, &s5});
        ASSERT_EQ(expectedS4S5, resultS4S5);        
    }
}

TEST(SeriesTests, TestBackshiftLookup) {
    PCSeries series0;
    series0.add(0, 0);
    
    PCSeries result00 = PCSeries::backshift(series0, Time(0), Space::minusInf());
    PCSeries &expected00 = series0;
    ASSERT_EQ(expected00, result00);
    
    PCSeries result01 = PCSeries::backshift(series0, Time(1), Space::minusInf());
    PCSeries expected01;
    expected01.add(Time(0), Space::minusInf());
    ASSERT_EQ(expected01, result01);
    
    PCSeries series1;
    series1.add(4, 5);
    series1.add(8, 1);
    series1.add(8, 1);
    series1.add(12, 3);
    series1.add(12, 1);
    
    PCSeries result10 = PCSeries::backshift(series1, Time(4), Space::minusInf());
    PCSeries expected10;    
    expected10.add(4, 1);
    expected10.add(4, 1);
    expected10.add(8, 3);
    expected10.add(8, 1);
    expected10.add(Time(12), Space::minusInf());
    expected10.add(Time(12), Space::minusInf());
    ASSERT_EQ(expected10, result10);
    
    PCSeries result11 = PCSeries::backshift(series1, Time(8), Space::minusInf());
    PCSeries expected11;    
    expected11.add(0, 1);
    expected11.add(4, 3);
    expected11.add(4, 1);
    expected11.add(Time(12), Space::minusInf());
    expected11.add(Time(12), Space::minusInf());
    ASSERT_EQ(expected11, result11);
}

TEST(SeriesTests, TestBackshiftMax) {
    PCSeries series0;
    series0.add(0, 0);
    
    PCSeries result00 = PCSeries::backshift(series0, ClosedIntvl(0, 0), SpaceMax{}, Space::minusInf());
    PCSeries &expected00 = series0;
    ASSERT_EQ(expected00, result00);
    
    PCSeries result01 = PCSeries::backshift(series0, ClosedIntvl(1, 1), SpaceMax{}, Space::minusInf());
    PCSeries expected01;
    expected01.add(Time(0), Space::minusInf());
    ASSERT_EQ(expected01, result01);
    
    PCSeries result02 = PCSeries::backshift(series0, ClosedIntvl(0, 2), SpaceMax{}, Space::minusInf());
    PCSeries &expected02 = series0;
    ASSERT_EQ(expected02, result02);
    
    PCSeries series1;
    series1.add(4, 3);
    series1.add(8, 1);
    series1.add(12, 5);
    series1.add(12, 5);
    
    PCSeries result10 = PCSeries::backshift(series1, ClosedIntvl(0, 1), SpaceMax{}, Space::minusInf());
    PCSeries expected10;
    expected10.add(4, 3);
    expected10.add(7, 1);
    expected10.add(12, 5);
    expected10.add(12, 5);
    ASSERT_EQ(expected10, result10);
    
    PCSeries result11 = PCSeries::backshift(series1, ClosedIntvl(0, 4), SpaceMax{}, Space::minusInf());
    PCSeries expected11;
    expected11.add(4, 3);
    expected11.add(12, 5);
    expected11.add(12, 5);
    ASSERT_EQ(expected11, result11);
    
    PCSeries result12 = PCSeries::backshift(series1, ClosedIntvl(0, 8), SpaceMax{}, Space::minusInf());
    PCSeries expected12;
    expected12.add(12, 5);
    expected12.add(12, 5);
    ASSERT_EQ(expected12, result12);
    
    PCSeries result13 = PCSeries::backshift(series1, ClosedIntvl(0, 9), SpaceMax{}, Space::minusInf());
    PCSeries expected13;
    expected13.add(12, 5);
    expected13.add(12, 5);
    ASSERT_EQ(expected13, result13);
    
    PCSeries series2;
    series2.add(4, 5);
    series2.add(8, 1);
    series2.add(8, 1);
    series2.add(12, 3);
    series2.add(12, 1);
    
    PCSeries result20 = PCSeries::backshift(series2, ClosedIntvl(0, 1), SpaceMax{}, Space::minusInf());
    PCSeries expected20;
    expected20.add(4, 5);
    expected20.add(7, 1);
    expected20.add(7, 1);
    expected20.add(12, 3);
    expected20.add(12, 1);
    ASSERT_EQ(expected20, result20);
    
    PCSeries result21 = PCSeries::backshift(series2, ClosedIntvl(0, 4), SpaceMax{}, Space::minusInf());
    PCSeries expected21;
    expected21.add(4, 5);
    expected21.add(4, 1);
    expected21.add(12, 3);
    expected21.add(12, 1);
    ASSERT_EQ(expected21, result21);
    
    PCSeries result22 = PCSeries::backshift(series2, ClosedIntvl(0, 8), SpaceMax{}, Space::minusInf());
    PCSeries expected22;
    expected22.add(4, 5);
    expected22.add(12, 3);
    expected22.add(12, 1);
    ASSERT_EQ(expected22, result22);
    
    PCSeries result23 = PCSeries::backshift(series2, ClosedIntvl(1, 5), SpaceMax{}, Space::minusInf());
    PCSeries expected23;
    expected23.add(3, 5);
    expected23.add(3, 1);
    expected23.add(11, 3);
    expected23.add(11, 1);
    expected23.add(Time(12), Space::minusInf());
    expected23.add(Time(12), Space::minusInf());
    ASSERT_EQ(expected23, result23);
    
    PCSeries result24 = PCSeries::backshift(series2, ClosedIntvl(4, 4), SpaceMax{}, Space::minusInf());
    PCSeries expected24;    
    expected24.add(4, 1);
    expected24.add(4, 1);
    expected24.add(8, 3);
    expected24.add(8, 1);
    expected24.add(Time(12), Space::minusInf());
    expected24.add(Time(12), Space::minusInf());
    ASSERT_EQ(expected24, result24);
    
    PCSeries result25 = PCSeries::backshift(series2, ClosedIntvl(8, 8), SpaceMax{}, Space::minusInf());
    PCSeries expected25;    
    expected25.add(0, 1);
    expected25.add(4, 3);
    expected25.add(4, 1);
    expected25.add(Time(12), Space::minusInf());
    expected25.add(Time(12), Space::minusInf());
    ASSERT_EQ(expected25, result25);
    
    PCSeries series3;
    series3.add(4, 3);
    series3.add(4, 3);
    series3.add(8, 1);
    series3.add(8, 1);
    series3.add(12, 3); 
    
    PCSeries result30 = PCSeries::backshift(series3, ClosedIntvl(0, 4), SpaceMax{}, Space::minusInf());
    PCSeries expected30;
    expected30.add(12, 3);
    ASSERT_EQ(expected30, result30);
}

TEST(SeriesTests, TestBackshiftMin) {
    PCSeries series0;
    series0.add(0, 0);
    
    PCSeries result00 = PCSeries::backshift(series0, ClosedIntvl(0, 0), SpaceMin{}, Space::inf());
    PCSeries &expected00 = series0;
    ASSERT_EQ(expected00, result00);
    
    PCSeries result01 = PCSeries::backshift(series0, ClosedIntvl(1, 1), SpaceMin{}, Space::inf());
    PCSeries expected01;
    expected01.add(Time(0), Space::inf());
    ASSERT_EQ(expected01, result01);
    
    PCSeries result02 = PCSeries::backshift(series0, ClosedIntvl(0, 2), SpaceMin{}, Space::inf());
    PCSeries &expected02 = series0;
    ASSERT_EQ(expected02, result02);
    
    PCSeries series1;
    series1.add(4, -3);
    series1.add(8, -1);
    series1.add(12, -5);
    series1.add(12, -5);
    
    PCSeries result10 = PCSeries::backshift(series1, ClosedIntvl(0, 1), SpaceMin{}, Space::inf());
    PCSeries expected10;
    expected10.add(4, -3);
    expected10.add(7, -1);
    expected10.add(12, -5);
    expected10.add(12, -5);
    ASSERT_EQ(expected10, result10);
    
    PCSeries result11 = PCSeries::backshift(series1, ClosedIntvl(0, 4), SpaceMin{}, Space::inf());
    PCSeries expected11;
    expected11.add(4, -3);
    expected11.add(12, -5);
    expected11.add(12, -5);
    ASSERT_EQ(expected11, result11);
    
    PCSeries result12 = PCSeries::backshift(series1, ClosedIntvl(0, 8), SpaceMin{}, Space::inf());
    PCSeries expected12;
    expected12.add(12, -5);
    expected12.add(12, -5);
    ASSERT_EQ(expected12, result12);
    
    PCSeries result13 = PCSeries::backshift(series1, ClosedIntvl(0, 9), SpaceMin{}, Space::inf());
    PCSeries expected13;
    expected13.add(12, -5);
    expected13.add(12, -5);
    ASSERT_EQ(expected13, result13);
    
    PCSeries series2;
    series2.add(4, -5);
    series2.add(8, -1);
    series2.add(8, -1);
    series2.add(12, -3);
    series2.add(12, -1);
    
    PCSeries result20 = PCSeries::backshift(series2, ClosedIntvl(0, 1), SpaceMin{}, Space::inf());
    PCSeries expected20;
    expected20.add(4, -5);
    expected20.add(7, -1);
    expected20.add(7, -1);
    expected20.add(12, -3);
    expected20.add(12, -1);
    ASSERT_EQ(expected20, result20);
    
    PCSeries result21 = PCSeries::backshift(series2, ClosedIntvl(0, 4), SpaceMin{}, Space::inf());
    PCSeries expected21;
    expected21.add(4, -5);
    expected21.add(4, -1);
    expected21.add(12, -3);
    expected21.add(12, -1);
    ASSERT_EQ(expected21, result21);
    
    PCSeries result22 = PCSeries::backshift(series2, ClosedIntvl(0, 8), SpaceMin{}, Space::inf());
    PCSeries expected22;
    expected22.add(4, -5);
    expected22.add(12, -3);
    expected22.add(12, -1);
    ASSERT_EQ(expected22, result22);
    
    PCSeries result23 = PCSeries::backshift(series2, ClosedIntvl(1, 5), SpaceMin{}, Space::inf());
    PCSeries expected23;
    expected23.add(3, -5);
    expected23.add(3, -1);
    expected23.add(11, -3);
    expected23.add(11, -1);
    expected23.add(Time(12), Space::inf());
    expected23.add(Time(12), Space::inf());
    ASSERT_EQ(expected23, result23);
    
    PCSeries result24 = PCSeries::backshift(series2, ClosedIntvl(4, 4), SpaceMin{}, Space::inf());
    PCSeries expected24;    
    expected24.add(4, -1);
    expected24.add(4, -1);
    expected24.add(8, -3);
    expected24.add(8, -1);
    expected24.add(Time(12), Space::inf());
    expected24.add(Time(12), Space::inf());
    ASSERT_EQ(expected24, result24);
    
    PCSeries result25 = PCSeries::backshift(series2, ClosedIntvl(8, 8), SpaceMin{}, Space::inf());
    PCSeries expected25;    
    expected25.add(0, -1);
    expected25.add(4, -3);
    expected25.add(4, -1);
    expected25.add(Time(12), Space::inf());
    expected25.add(Time(12), Space::inf());
    ASSERT_EQ(expected25, result25);
    
    PCSeries series3;
    series3.add(4, -3);
    series3.add(4, -3);
    series3.add(8, -1);
    series3.add(8, -1);
    series3.add(12, -3); 
    
    PCSeries result30 = PCSeries::backshift(series3, ClosedIntvl(0, 4), SpaceMin{}, Space::inf());
    PCSeries expected30;
    expected30.add(12, -3);
    ASSERT_EQ(expected30, result30);
}

TEST(SeriesTests, TestBackshiftOr) {
    PCSeries series0;
    series0.add(4, 0);
    series0.add(8, 1);
    series0.add(12, 0);
    series0.add(12, 1);
    
    PCSeries result01 = PCSeries::backshift(series0, ClosedIntvl(0, 2), SpaceOr{}, Space(0));
    PCSeries expected01;
    expected01.add(2, 0);
    expected01.add(8, 1);
    expected01.add(10, 0);
    expected01.add(12, 1);
    expected01.add(12, 1);
    ASSERT_EQ(expected01, result01);
    
    PCSeries result02 = PCSeries::backshift(series0, ClosedIntvl(0, 4), SpaceOr{}, Space(0));
    PCSeries expected02;
    expected02.add(12, 1);
    expected02.add(12, 1);
    ASSERT_EQ(expected02, result02);
}

TEST(SeriesTests, TestBackshiftAnd) {
    PCSeries series0;
    series0.add(4, 1);
    series0.add(8, 0);
    series0.add(12, 1);
    series0.add(12, 0);
    
    PCSeries result01 = PCSeries::backshift(series0, ClosedIntvl(0, 2), SpaceAnd{}, Space(1));
    PCSeries expected01;
    expected01.add(2, 1);
    expected01.add(8, 0);
    expected01.add(10, 1);
    expected01.add(12, 0);
    expected01.add(12, 0);
    ASSERT_EQ(expected01, result01);
    
    PCSeries result02 = PCSeries::backshift(series0, ClosedIntvl(0, 4), SpaceAnd{}, Space(1));
    PCSeries expected02;
    expected02.add(12, 0);
    expected02.add(12, 0);
    ASSERT_EQ(expected02, result02);
}

TEST(SeriesTests, TestUntilMin) {
    {
        PCSeries series01;
        series01.add(0, 0);
        
        PCSeries series02;
        series02.add(0, 0);
        
        PCSeries result0 = PCMonitorVisitor::untimedUntil(series01, series02, SpaceMin{}, Space(-1));
        PCSeries expected0;
        expected0.add(0, -1);
        ASSERT_EQ(expected0, result0);
    }    
    {
        PCSeries series11;
        series11.add(0, 0);
        
        PCSeries series12;
        series12.add(0, 1);
        
        PCSeries result1 = PCMonitorVisitor::untimedUntil(series11, series12, SpaceMin{}, Space(-1));
        PCSeries expected1;
        expected1.add(0, 0);
        ASSERT_EQ(expected1, result1);
    }    
    {
        PCSeries series21;
        series21.add(2, 1);
        series21.add(4, 2);
        series21.add(5, 3);
        series21.add(6, 4);
        series21.add(6, 4);
        
        PCSeries series22;
        series22.add(1, 0);
        series22.add(3, 1);
        series22.add(6, 0);
        series22.add(6, 0);
        
        PCSeries result2 = PCMonitorVisitor::untimedUntil(series21, series22, SpaceMin{}, Space(-1));
        PCSeries expected2;
        expected2.add(2, 1);
        expected2.add(3, 2);
        expected2.add(6, -1);
        expected2.add(6, -1);
        ASSERT_EQ(expected2, result2);
    }
    {
        PCSeries series31;
        series31.add(1, 2);
        series31.add(2, 1);
        series31.add(4, 3);
        series31.add(6, 5);
        series31.add(8, 4);
        series31.add(9, 6);
        
        PCSeries series32;
        series32.add(3, 0);
        series32.add(5, 1);
        series32.add(7, 2);
        series32.add(9, 0);
        
        PCSeries result3 = PCMonitorVisitor::untimedUntil(series31, series32, SpaceMin{}, Space(-1));
        PCSeries expected3;
        expected3.add(2, 1);
        expected3.add(4, 3);
        expected3.add(6, 5);
        expected3.add(7, 4);
        expected3.add(9, -1);
        
        ASSERT_EQ(expected3, result3);
    }
    {
        PCSeries series41;
        series41.add(1, 3);
        series41.add(2, 1);
        series41.add(2, 2);
        series41.add(3, 1);
        series41.add(4, 2);
        series41.add(4, 1);
        
        PCSeries series42;
        series42.add(0, 1);
        series42.add(2, 0);
        series42.add(3, 1);
        series42.add(3, 2);
        series42.add(4, 0);
        series42.add(4, 1);
        
        PCSeries result4 = PCMonitorVisitor::untimedUntil(series41, series42, SpaceMin{}, Space(-1));
        PCSeries expected4;
        expected4.add(0, 3);
        expected4.add(2, 1);
        expected4.add(2, 2);
        expected4.add(3, 1);
        expected4.add(3, 2);
        expected4.add(4, 1);
        expected4.add(4, 1);
        ASSERT_EQ(expected4, result4);
    }
}

TEST(SeriesTests, TestUntilFirst) {
    {
        PCSeries series01;
        series01.add(0, 0);
        
        PCSeries series02;
        series02.add(0, 0);
        
        PCSeries result0 = PCMonitorVisitor::untimedUntil(series01, series02, SpaceFirst{}, Space(-1));
        PCSeries expected0;
        expected0.add(0, -1);
        ASSERT_EQ(expected0, result0);
    }    
    {
        PCSeries series11;
        series11.add(0, 0);
        
        PCSeries series12;
        series12.add(0, 1);
        
        PCSeries result1 = PCMonitorVisitor::untimedUntil(series11, series12, SpaceFirst{}, Space(-1));
        PCSeries expected1;
        expected1.add(0, 0);
        ASSERT_EQ(expected1, result1);
    }    
    {
        PCSeries series21;
        series21.add(2, 1);
        series21.add(4, 2);
        series21.add(5, 3);
        series21.add(6, 4);
        series21.add(6, 4);
        
        PCSeries series22;
        series22.add(1, 0);
        series22.add(3, 1);
        series22.add(6, 0);
        series22.add(6, 0);
        
        PCSeries result2 = PCMonitorVisitor::untimedUntil(series21, series22, SpaceFirst{}, Space(-1));
        PCSeries expected2;
        expected2.add(2, 1);
        expected2.add(3, 2);
        expected2.add(6, -1);
        expected2.add(6, -1);
        ASSERT_EQ(expected2, result2);
    }
    {
        PCSeries series31;
        series31.add(1, 2);
        series31.add(2, 1);
        series31.add(4, 3);
        series31.add(6, 5);
        series31.add(8, 4);
        series31.add(9, 6);
        
        PCSeries series32;
        series32.add(3, 0);
        series32.add(5, 1);
        series32.add(7, 2);
        series32.add(9, 0);
        
        PCSeries result3 = PCMonitorVisitor::untimedUntil(series31, series32, SpaceFirst{}, Space(-1));
        PCSeries expected3;
        expected3.add(4, 3);
        expected3.add(6, 5);
        expected3.add(7, 4);
        expected3.add(9, -1);
        
        ASSERT_EQ(expected3, result3);
    }
    {
        PCSeries series41;
        series41.add(1, 3);
        series41.add(2, 1);
        series41.add(2, 2);
        series41.add(3, 1);
        series41.add(4, 2);
        series41.add(4, 1);
        
        PCSeries series42;
        series42.add(0, 1);
        series42.add(2, 0);
        series42.add(3, 1);
        series42.add(3, 2);
        series42.add(4, 0);
        series42.add(4, 1);
        
        PCSeries result4 = PCMonitorVisitor::untimedUntil(series41, series42, SpaceFirst{}, Space(-1));
        PCSeries expected4;
        expected4.add(0, 3);
        expected4.add(2, 2);
        expected4.add(2, 2);
        expected4.add(3, 1);
        expected4.add(3, 2);
        expected4.add(4, 1);
        expected4.add(4, 1);
        ASSERT_EQ(expected4, result4);
    }
}

TEST(SeriesTests, TestReverse) {
    {
        PCSeries series;
        series.add(0, 0);
        
        PCSeries expected;
        expected.add(0, 0);
        
        PCSeries rev(PCSeries::reverse(series));
        ASSERT_EQ(expected, rev);
        
        PCSeries revrev(PCSeries::reverse(rev, !series.closed()));
        ASSERT_EQ(series, revrev);
    }
    {
        PCSeries series;
        series.add(1, 1);
        series.add(2, 2);
        
        PCSeries expected;
        expected.add(1, 2);
        expected.add(1, 2);
        expected.add(2, 1);
        expected.add(2, 1);
        
        PCSeries rev(PCSeries::reverse(series));
        ASSERT_EQ(expected, rev);
        
        PCSeries revrev(PCSeries::reverse(rev, !series.closed()));
        ASSERT_EQ(series, revrev);
    }
    {
        PCSeries series;
        series.add(1, 1);
        series.add(1, 1);
        series.add(2, 2);
        series.add(2, 2);
        
        PCSeries expected;
        expected.add(1, 2);
        expected.add(2, 1);
        expected.add(2, 1);
        
        PCSeries rev(PCSeries::reverse(series));
        ASSERT_EQ(expected, rev);
        
        PCSeries revrev(PCSeries::reverse(rev, !series.closed()));
        ASSERT_EQ(series, revrev);
    }
    {
        PCSeries series;
        series.add(1, 0);
        series.add(2, 1);
        series.add(2, 2);
        series.add(3, 3);
        series.add(4, 4);
        series.add(4, 5);
        
        PCSeries expected;
        expected.add(0, 5);
        expected.add(1, 4);
        expected.add(1, 4);
        expected.add(2, 3);
        expected.add(2, 2);
        expected.add(3, 1);
        expected.add(3, 1);
        expected.add(4, 0);
        expected.add(4, 0);
        
        PCSeries rev(PCSeries::reverse(series));
        ASSERT_EQ(expected, rev);
        
        PCSeries revrev(PCSeries::reverse(rev, !series.closed()));
        ASSERT_EQ(series, revrev);        
    }
}
