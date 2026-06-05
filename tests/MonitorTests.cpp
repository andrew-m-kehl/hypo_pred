#include "gtest/gtest.h"

#include "Stle/PCMonitor.hpp"
#include "Stle/CsvPCSignalReader.hpp"
#include "Stle/SExprParser.hpp"
#include "Stle/PCRobMonitor.hpp"

#include "Stle/Cmd/PCState.hpp"
#include "Stle/Cmd/CmdRunner.hpp"
#include "Stle/Cmd/CmdRunnerVisitor.hpp"

#include "Stle/U/CsvReader.hpp"
#include "Stle/U/FileReader.hpp"
#include "Stle/U/SLexer.hpp"
#include "Stle/U/StrPrinter.hpp"

#include <vector>

using namespace Stle;

TEST(MonitorTests, TestConstVar) {
    PCSeries series;
    series.add(1, 0);
    series.add(2, 1);
    series.add(3, 0);
    series.add(4, 1);
    series.add(4, 1);
    
    PCSignal signal(std::move(series));    
    SignalVars vars(VARS_XN, 1);
    
    ExprSet s;    
    PCMonitor monitor(signal, vars, s);
    
    {
        ExprPtr e = s.makeConst(0);
        PCSeries expected;
        expected.add(4, 0);
        expected.add(4, 0);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeConst(2);
        PCSeries expected;
        expected.add(4, 2);
        expected.add(4, 2);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeBoolConst(false);
        PCSeries expected;
        expected.add(4, 0);
        expected.add(4, 0);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeBoolConst(true);
        PCSeries expected;
        expected.add(4, 1);
        expected.add(4, 1);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeVar("x0");
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(signal[0], output);    
    }
}

TEST(MonitorTests, TestApply) {
    PCSeries series0;
    series0.add(1, -1);
    series0.add(2, 2);
    series0.add(3, -3);
    series0.add(4, 4);
    series0.add(4, -4);
    
    PCSeries series1;
    series1.add(1, 0);
    series1.add(2, 2);
    series1.add(3, 0);
    series1.add(4, 2);
    series1.add(4, 2);
    
    PCSeries series2;
    series2.add(1, 0);
    series2.add(2, 1);
    series2.add(3, 0);
    series2.add(4, 1);
    series2.add(4, 1);
    
    std::vector<PCSeries> signal0V;
    signal0V.emplace_back(std::move(series0));
    signal0V.emplace_back(std::move(series1));
    signal0V.emplace_back(std::move(series2));
    PCSignal signal0(std::move(signal0V));    
    SignalVars vars0(VARS_XN, signal0.size());
    
    ExprSet s;
    PCMonitor monitor(signal0, vars0, s);
    
    ExprPtr e0 = s.makeConst(0);
    ExprPtr eX0 = s.makeVar("x0");
    ExprPtr eX1 = s.makeVar("x1");    
    
    {
        // Comparisons.
        PCSeries expectedX1Gt0;
        expectedX1Gt0.add(1, 0);
        expectedX1Gt0.add(2, 1);
        expectedX1Gt0.add(3, 0);
        expectedX1Gt0.add(4, 1);
        expectedX1Gt0.add(4, 1);
        ExprPtr eX1Gt0 = s.makeApply(APPLY_GT, {eX1, e0});
        const PCSeries &outputX1Gt0 = monitor.makeOutput(eX1Gt0);
        ASSERT_EQ(expectedX1Gt0, outputX1Gt0);
    }    
    {
        // Abs.
        PCSeries expectedAbsX0;
        expectedAbsX0.add(1, 1);
        expectedAbsX0.add(2, 2);
        expectedAbsX0.add(3, 3);
        expectedAbsX0.add(4, 4);
        expectedAbsX0.add(4, 4);
        
        ExprPtr eAbsX0 = s.makeApply(APPLY_ABS, {eX0});
        const PCSeries &outputAbsX0 = monitor.makeOutput(eAbsX0);
        ASSERT_EQ(expectedAbsX0, outputAbsX0);        
        PCSeries expectedAbsX0MinusX1GtX0;    
        expectedAbsX0MinusX1GtX0.add(2, 1);
        expectedAbsX0MinusX1GtX0.add(4, 3);
        expectedAbsX0MinusX1GtX0.add(4, 3);
        ExprPtr eX1Gt0 = s.makeApply(APPLY_GT, {eX1, e0});
        ExprPtr eAbsX0MinusX1GtX0 = s.makeApply(APPLY_MINUS,  {eAbsX0, eX1Gt0});
        const PCSeries &outputAbsX0MinusX1GtX0 = monitor.makeOutput(eAbsX0MinusX1GtX0);
        ASSERT_EQ(expectedAbsX0MinusX1GtX0, outputAbsX0MinusX1GtX0);
        
        PCSeries expectedMinusAbsX0;
        expectedMinusAbsX0.add(1, -1);
        expectedMinusAbsX0.add(2, -2);
        expectedMinusAbsX0.add(3, -3);
        expectedMinusAbsX0.add(4, -4);
        expectedMinusAbsX0.add(4, -4);
        ExprPtr eMinusAbsX0 = s.makeApply(APPLY_MINUS, {eAbsX0});
        const PCSeries &outputMinusAbsX0 = monitor.makeOutput(eMinusAbsX0);
        ASSERT_EQ(expectedMinusAbsX0, outputMinusAbsX0);
    }
    {
        // Not.
        PCSeries expectedNotX1;
        expectedNotX1.add(1, 1);
        expectedNotX1.add(2, 0);
        expectedNotX1.add(3, 1);
        expectedNotX1.add(4, 0);
        expectedNotX1.add(4, 0);
        ExprPtr eNotX1 = s.makeApply(APPLY_BOOL_NOT, {eX1});
        const PCSeries &resultNotX1 = monitor.makeOutput(eNotX1);
        ASSERT_EQ(expectedNotX1, resultNotX1);
    }
    {
        // Addition.
        PCSeries expectedX1PlusX1;
        expectedX1PlusX1.add(1, 0);
        expectedX1PlusX1.add(2, 4);
        expectedX1PlusX1.add(3, 0);
        expectedX1PlusX1.add(4, 4);
        expectedX1PlusX1.add(4, 4);
        ExprPtr eX1PlusX1 = s.makeApply(APPLY_PLUS, {eX1, eX1});
        const PCSeries &resultX1PlusX1 = monitor.makeOutput(eX1PlusX1);
        ASSERT_EQ(expectedX1PlusX1, resultX1PlusX1);
    }
    {
        // Multiplication.
        PCSeries expectedX0MulX1;
        expectedX0MulX1.add(1, 0);
        expectedX0MulX1.add(2, 4);
        expectedX0MulX1.add(3, 0);
        expectedX0MulX1.add(4, 8);
        expectedX0MulX1.add(4, -8);
        ExprPtr eX0MulX1 = s.makeApply(APPLY_MUL, {eX0, eX1});
        const PCSeries &resultX0MulX1 = monitor.makeOutput(eX0MulX1);
        ASSERT_EQ(expectedX0MulX1, resultX0MulX1);
    }    
    {
        // Division.
        PCSeries expectedX1DivX0;
        expectedX1DivX0.add(1, 0);
        expectedX1DivX0.add(2, 1);
        expectedX1DivX0.add(3, 0);
        expectedX1DivX0.add(4, 0.5);
        expectedX1DivX0.add(4, -0.5);
        ExprPtr eX1DivX0 = s.makeApply(APPLY_DIV, {eX1, eX0});
        const PCSeries &resultX1DivX0 = monitor.makeOutput(eX1DivX0);
        ASSERT_EQ(expectedX1DivX0, resultX1DivX0);
    }
    
    {
        // Derivative.
        PCSeries expectedX0Deriv;
        expectedX0Deriv.add(1, 3);
        expectedX0Deriv.add(2,-5);
        expectedX0Deriv.add(3, 7);
        expectedX0Deriv.add(3, -8);
        expectedX0Deriv.add(Time(4), Space::minusInf());
        expectedX0Deriv.add(Time(4), Space::minusInf());
        ExprPtr eX0Deriv = s.makeApply(APPLY_DER, {eX0});
        const PCSeries &resultX0Deriv = monitor.makeOutput(eX0Deriv);
        ASSERT_EQ(expectedX0Deriv, resultX0Deriv);

        // Integral.
        PCSeries expectedX0Integ;
        expectedX0Integ.add(1, -1);
        expectedX0Integ.add(2, 1);
        expectedX0Integ.add(3, -2);
        expectedX0Integ.add(4, 2);
        expectedX0Integ.add(4, -2);
        ExprPtr eX0Integ = s.makeApply(APPLY_INT, {eX0});
        const PCSeries& resultX0Integ = monitor.makeOutput(eX0Integ);
        ASSERT_EQ(expectedX0Integ, resultX0Integ);
    }

    // TODO: Perhaps more tests here: constant as operands, 0 arguments, more than two arguments, etc.
}

TEST(MonitorTests, TestLookup) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series0;
        series0.add(0, 0);    
        PCSignal signal0(std::move(series0));
        
        PCMonitor m(signal0, vars, s);
        
        ExprPtr e0 = s.makeLookup(Time(0), Space::minusInf(), x0);
        const PCSeries &result00 = m.makeOutput(e0);
        PCSeries expected00;
        expected00.add(0, 0);
        ASSERT_EQ(expected00, result00);
        
        ExprPtr e1 = s.makeLookup(Time(1), Space::minusInf(), x0);
        const PCSeries &result01 = m.makeOutput(e1);
        PCSeries expected01;
        expected01.add(Time(0), Space::minusInf());
        ASSERT_EQ(expected01, result01);
    }
    
    {
        PCSeries series1;
        series1.add(4, 5);
        series1.add(8, 1);
        series1.add(8, 1);
        series1.add(12, 3);
        series1.add(12, 1);
        PCSignal signal1(std::move(series1));
        
        PCMonitor m(signal1, vars, s);
        
        ExprPtr e0 = s.makeLookup(Time(4), Space::minusInf(), x0);
        const PCSeries &result10 = m.makeOutput(e0);
        PCSeries expected10;    
        expected10.add(4, 1);
        expected10.add(4, 1);
        expected10.add(8, 3);
        expected10.add(8, 1);
        expected10.add(Time(12), Space::minusInf());
        expected10.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected10, result10);
        
        ExprPtr e1 = s.makeLookup(Time(8), Space(-1), x0);
        const PCSeries &result11 = m.makeOutput(e1);
        PCSeries expected11;    
        expected11.add(0, 1);
        expected11.add(4, 3);
        expected11.add(4, 1);
        expected11.add(Time(12), Space(-1));
        expected11.add(Time(12), Space(-1));
        ASSERT_EQ(expected11, result11);
    }
}

TEST(MonitorTests, TestOnMax) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series;        
        series.add(0, 0);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 0), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(0, 0);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(1, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(Time(0), Space::minusInf());
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 2), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(0, 0);
        ASSERT_EQ(expected2, result2);
    }
    
    {
        PCSeries series;
        series.add(4, 3);
        series.add(8, 1);
        series.add(12, 5);
        series.add(12, 5);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 3);
        expected0.add(7, 1);
        expected0.add(12, 5);
        expected0.add(12, 5);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, 3);
        expected1.add(12, 5);
        expected1.add(12, 5);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 8), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(12, 5);
        expected2.add(12, 5);
        ASSERT_EQ(expected2, result2);
        
        ExprPtr e3 = s.makeOn(ClosedIntvl(0, 9), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(12, 5);
        expected3.add(12, 5);
        ASSERT_EQ(expected3, result3);
    }
    
    {
        PCSeries series;
        series.add(4, 5);
        series.add(8, 1);
        series.add(8, 1);
        series.add(12, 3);
        series.add(12, 1);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 5);
        expected0.add(7, 1);
        expected0.add(7, 1);
        expected0.add(12, 3);
        expected0.add(12, 1);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, 5);
        expected1.add(4, 1);
        expected1.add(12, 3);
        expected1.add(12, 1);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 8), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result22 = m.makeOutput(e2);
        PCSeries expected22;
        expected22.add(4, 5);
        expected22.add(12, 3);
        expected22.add(12, 1);
        ASSERT_EQ(expected22, result22);
        
        ExprPtr e3 = s.makeOn(ClosedIntvl(1, 5), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(3, 5);
        expected3.add(3, 1);
        expected3.add(11, 3);
        expected3.add(11, 1);
        expected3.add(Time(12), Space::minusInf());
        expected3.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected3, result3);
        
        ExprPtr e4 = s.makeOn(ClosedIntvl(4, 4), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result4 = m.makeOutput(e4);
        PCSeries expected4;    
        expected4.add(4, 1);
        expected4.add(4, 1);
        expected4.add(8, 3);
        expected4.add(8, 1);
        expected4.add(Time(12), Space::minusInf());
        expected4.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected4, result4);
        
        ExprPtr e5 = s.makeOn(ClosedIntvl(8, 8), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result5 = m.makeOutput(e5);
        PCSeries expected5;    
        expected5.add(0, 1);
        expected5.add(4, 3);
        expected5.add(4, 1);
        expected5.add(Time(12), Space::minusInf());
        expected5.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected5, result5);
    }
    
    {
        PCSeries series;
        series.add(4, 3);
        series.add(4, 3);
        series.add(8, 1);
        series.add(8, 1);
        series.add(12, 3); 
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MAX, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(12, 3);
        ASSERT_EQ(expected0, result0);
    }
}

TEST(MonitorTests, TestPastOnMax) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series;        
        series.add(0, 0);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(-1, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        PCSeries expected0;
        expected0.add(0, 0);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(expected0, result0);
    }
    {
        PCSeries series;        
        series.add(1, 0);
        series.add(2, 3);
        series.add(3, 1);
        series.add(4, 2);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(-1, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        PCSeries expected0;
        expected0.add(3, 3);
        expected0.add(4, 2);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(expected0, result0);
    }
    {
        PCSeries series;        
        series.add(0, 3);
        series.add(2, 0);
        series.add(2, 2);
        series.add(4, 1);
        series.add(5, 4);
        series.add(5, 5);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(-1, 1), s.makeAggregate(AGGREGATE_MAX, x0));
        PCSeries expected0;
        expected0.add(1, 3);
        expected0.add(1, 3);
        expected0.add(3, 2);
        expected0.add(4, 4);
        expected0.add(5, 5);
        expected0.add(5, 5);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(expected0, result0);
    }
}

TEST(MonitorTests, TestOnMin) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series;        
        series.add(0, 0);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 0), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(0, 0);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(1, 1), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(Time(0), Space::inf());
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 2), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(0, 0);
        ASSERT_EQ(expected2, result2);
    }
    
    {
        PCSeries series;
        series.add(4, -3);
        series.add(8, -1);
        series.add(12, -5);
        series.add(12, -5);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 1), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, -3);
        expected0.add(7, -1);
        expected0.add(12, -5);
        expected0.add(12, -5);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, -3);
        expected1.add(12, -5);
        expected1.add(12, -5);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 8), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(12, -5);
        expected2.add(12, -5);
        ASSERT_EQ(expected2, result2);
        
        ExprPtr e3 = s.makeOn(ClosedIntvl(0, 9), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(12, -5);
        expected3.add(12, -5);
        ASSERT_EQ(expected3, result3);
    }
    
    {
        PCSeries series;
        series.add(4, -5);
        series.add(8, -1);
        series.add(8, -1);
        series.add(12,- 3);
        series.add(12, -1);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 1), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, -5);
        expected0.add(7, -1);
        expected0.add(7, -1);
        expected0.add(12, -3);
        expected0.add(12, -1);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, -5);
        expected1.add(4, -1);
        expected1.add(12, -3);
        expected1.add(12, -1);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeOn(ClosedIntvl(0, 8), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(4, -5);
        expected2.add(12, -3);
        expected2.add(12, -1);
        ASSERT_EQ(expected2, result2);
        
        ExprPtr e3 = s.makeOn(ClosedIntvl(1, 5), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(3, -5);
        expected3.add(3, -1);
        expected3.add(11, -3);
        expected3.add(11, -1);
        expected3.add(Time(12), Space::inf());
        expected3.add(Time(12), Space::inf());
        ASSERT_EQ(expected3, result3);
        
        ExprPtr e4 = s.makeOn(ClosedIntvl(4, 4), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result4 = m.makeOutput(e4);
        PCSeries expected4;    
        expected4.add(4, -1);
        expected4.add(4, -1);
        expected4.add(8, -3);
        expected4.add(8, -1);
        expected4.add(Time(12), Space::inf());
        expected4.add(Time(12), Space::inf());
        ASSERT_EQ(expected4, result4);
        
        ExprPtr e5 = s.makeOn(ClosedIntvl(8, 8), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result5 = m.makeOutput(e5);
        PCSeries expected5;    
        expected5.add(0, -1);
        expected5.add(4, -3);
        expected5.add(4, -1);
        expected5.add(Time(12), Space::inf());
        expected5.add(Time(12), Space::inf());
        ASSERT_EQ(expected5, result5);
    }
    
    {
        PCSeries series;
        series.add(4, -3);
        series.add(4, -3);
        series.add(8, -1);
        series.add(8, -1);
        series.add(12, -3); 
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeOn(ClosedIntvl(0, 4), s.makeAggregate(AGGREGATE_MIN, x0));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(12, -3);
        ASSERT_EQ(expected0, result0);
    }
}

TEST(MonitorTests, TestF) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    PCSeries series0;
    series0.add(4, 0);
    series0.add(6, 1);
    series0.add(8, 2);
    series0.add(12, 0);
    series0.add(12, 1);
    PCSignal signal0(std::move(series0));
    
    PCMonitor m(signal0, vars, s);
    
    ExprPtr e0  = s.makeTemporal(ClosedIntvl(0, 2), TEMPORAL_F, x0);
    const PCSeries &result01 = m.makeOutput(e0);
    PCSeries expected01;
    expected01.add(2, 0);
    expected01.add(8, 1);
    expected01.add(10, 0);
    expected01.add(12, 1);
    expected01.add(12, 1);
    ASSERT_EQ(expected01, result01);
    
    ExprPtr e1  = s.makeTemporal(ClosedIntvl(0, 4), TEMPORAL_F, x0);
    const PCSeries &result02 = m.makeOutput(e1);
    PCSeries expected02;
    expected02.add(12, 1);
    expected02.add(12, 1);
    ASSERT_EQ(expected02, result02);
}

TEST(MonitorTests, TestG) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    PCSeries series0;
    series0.add(2, 1);
    series0.add(4, 2);
    series0.add(8, 0);
    series0.add(12, 1);
    series0.add(12, 0);
    PCSignal signal0(std::move(series0));
    
    PCMonitor m(signal0, vars, s);
    
    ExprPtr e0  = s.makeTemporal(ClosedIntvl(0, 2), TEMPORAL_G, x0);
    const PCSeries &result01 = m.makeOutput(e0);
    PCSeries expected01;
    expected01.add(2, 1);
    expected01.add(8, 0);
    expected01.add(10, 1);
    expected01.add(12, 0);
    expected01.add(12, 0);
    ASSERT_EQ(expected01, result01);
    
    ExprPtr e1  = s.makeTemporal(ClosedIntvl(0, 4), TEMPORAL_G, x0);
    const PCSeries &result02 = m.makeOutput(e1);
    PCSeries expected02;
    expected02.add(12, 0);
    expected02.add(12, 0);
    ASSERT_EQ(expected02, result02);
}

TEST(MonitorTests, TestPastF) {
    {
        SignalVars vars(VARS_XN, 1);
        ExprSet s;
        ExprPtr x0 = s.makeVar("x0");
        
        PCSeries series;
        series.add(4, 0);
        series.add(6, 1);
        series.add(8, 2);
        series.add(12, 0);
        series.add(12, 1);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0  = s.makeTemporal(ClosedIntvl(0, 0), TEMPORAL_F, x0);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(signal[0], result0);
        
        ExprPtr e1  = s.makeTemporal(ClosedIntvl(-2, 0), TEMPORAL_F, x0);        
        PCSeries expected1;
        expected1.add(4, 0);
        expected1.add(10, 1);
        expected1.add(12, 0);
        expected1.add(12, 1);        
        const PCSeries &result1 = m.makeOutput(e1);        
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2  = s.makeTemporal(ClosedIntvl(-4, 0), TEMPORAL_F, x0);        
        PCSeries expected2;
        expected2.add(4, 0);
        expected2.add(12, 1);
        expected2.add(12, 1);        
        const PCSeries &result2 = m.makeOutput(e2);        
        ASSERT_EQ(expected2, result2);
        
        ExprPtr e3  = s.makeTemporal(ClosedIntvl(-1, 1), TEMPORAL_F, x0);        
        PCSeries expected3;
        expected3.add(3, 0);
        expected3.add(9, 1);
        expected3.add(11, 0);
        expected3.add(12, 1);
        expected3.add(12, 1);
        const PCSeries &result3 = m.makeOutput(e3);
        ASSERT_EQ(expected3, result3);
    }
}

TEST(MonitorTests, TestPastG) {
    {
        SignalVars vars(VARS_XN, 1);
        ExprSet s;
        ExprPtr x0 = s.makeVar("x0");
        
        PCSeries series;
        series.add(4, 1);        
        series.add(8, 0);
        series.add(12, 1);
        series.add(12, 0);
        PCSignal signal(std::move(series));
        
        PCMonitor m(signal, vars, s);
        
        ExprPtr e0  = s.makeTemporal(ClosedIntvl(0, 0), TEMPORAL_G, x0);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(signal[0], result0);
        
        ExprPtr e1  = s.makeTemporal(ClosedIntvl(-2, 0), TEMPORAL_G, x0);
        
        PCSeries expected1;
        expected1.add(4, 1);
        expected1.add(10, 0);
        expected1.add(12, 1);
        expected1.add(12, 0);
        
        const PCSeries &result1 = m.makeOutput(e1);        
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2  = s.makeTemporal(ClosedIntvl(-4, 0), TEMPORAL_G, x0);
        
        PCSeries expected2;
        expected2.add(4, 1);
        expected2.add(12, 0);
        expected2.add(12, 0);
        
        const PCSeries &result2 = m.makeOutput(e2);        
        ASSERT_EQ(expected2, result2);
    }
}

TEST(MonitorTests, TestUntilMin) {
    SignalVars vars(VARS_XN, 2);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    ExprPtr x1 = s.makeVar("x1");
    ExprPtr untilExpr = s.makeUntil(ClosedIntvl(Time(0), Time::inf()), Space(-1), s.makeAggregate(AGGREGATE_MIN, x0), x1);
    
    {
        PCSeries series01;
        series01.add(0, 0);
        
        PCSeries series02;
        series02.add(0, 0);
        
        std::vector<PCSeries> signal0V;
        signal0V.emplace_back(std::move(series01));
        signal0V.emplace_back(std::move(series02));
        
        PCSignal signal0(std::move(signal0V));
        PCMonitor m(signal0, vars, s);
        
        const PCSeries &result0 = m.makeOutput(untilExpr);
        PCSeries expected0;
        expected0.add(0, -1);
        ASSERT_EQ(expected0, result0);
    }    
    {
        PCSeries series11;
        series11.add(0, 0);
        
        PCSeries series12;
        series12.add(0, 1);
        
        std::vector<PCSeries> signal1V;
        signal1V.emplace_back(std::move(series11));
        signal1V.emplace_back(std::move(series12));
        
        PCSignal signal1(std::move(signal1V));
        PCMonitor m(signal1, vars, s);
        
        const PCSeries &result1 = m.makeOutput(untilExpr);
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
        
        std::vector<PCSeries> signal2V;
        signal2V.emplace_back(std::move(series21));
        signal2V.emplace_back(std::move(series22));
        
        PCSignal signal2(std::move(signal2V));
        PCMonitor m(signal2, vars, s);
        
        const PCSeries &result2 = m.makeOutput(untilExpr);
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
        
        std::vector<PCSeries> signal3V;
        signal3V.emplace_back(std::move(series31));
        signal3V.emplace_back(std::move(series32));
        
        PCSignal signal3(std::move(signal3V));
        PCMonitor m(signal3, vars, s);
        
        const PCSeries &result3 = m.makeOutput(untilExpr);
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
        
        std::vector<PCSeries> signal4V;
        signal4V.emplace_back(std::move(series41));
        signal4V.emplace_back(std::move(series42));
        
        PCSignal signal4(std::move(signal4V));
        PCMonitor m(signal4, vars, s);
        
        const PCSeries &result4 = m.makeOutput(untilExpr);
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

// TODO: Need tests for past until.

TEST(MonitorTests, TestRequestResponse) {
    {
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse01.csv", "r");
        PCSignal signal = readPCSignalCsv(csvReader);
        ExprSet es;
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(es, U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse0.stl", "r");
        ExprPtr expr = parser.parse();
        SignalVars vars(VARS_XN, signal.size());
        PCMonitor monitor(signal, vars, es);
        const PCSeries &outSeries = monitor.makeOutput(expr);
        Space result = outSeries[0].value();
        Space expected(1);
        ASSERT_EQ(expected, result);
    }
    {
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse01.csv", "r");
        PCSignal signal = readPCSignalCsv(csvReader);
        ExprSet es;
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(es, U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse1.stl", "r");
        ExprPtr expr = parser.parse();
        SignalVars vars(VARS_XN, signal.size());
        PCMonitor monitor(signal, vars, es);
        const PCSeries &outSeries = monitor.makeOutput(expr);
        Space result = outSeries[0].value();
        Space expected(0);
        ASSERT_EQ(expected, result);
    }
    {
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse23.csv", "r");
        PCSignal signal = readPCSignalCsv(csvReader);
        ExprSet es;
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(es, U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse2.stl", "r");
        ExprPtr expr = parser.parse();
        SignalVars vars(VARS_XN, signal.size());
        PCMonitor monitor(signal, vars, es);
        const PCSeries &outSeries = monitor.makeOutput(expr);
        Space result = outSeries[0].value();
        Space expected(1);
        ASSERT_EQ(expected, result);
    }
    {
        U::CsvReader<U::FileReader<U::File>> csvReader(U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse23.csv", "r");
        PCSignal signal = readPCSignalCsv(csvReader);
        ExprSet es;
        SExprParser<U::SLexer<U::FileReader<U::File>>> parser(es, U::OF_READER, U::OF_FILE, "tests/data/MonitorTests/TestRequestResponse3.stl", "r");
        ExprPtr expr = parser.parse();
        SignalVars vars(VARS_XN, signal.size());
        PCMonitor monitor(signal, vars, es);
        const PCSeries &outSeries = monitor.makeOutput(expr);
        Space result = outSeries[0].value();
        Space expected(0);
        ASSERT_EQ(expected, result);
    }
}

TEST(MonitorTests, TestReverse) {
    {
        PCSeries series0;
        series0.add(4, 0);
        series0.add(4, 0);        

        PCSeries series1;
        series1.add(1, 0);
        series1.add(2, 1);
        series1.add(2, 2);
        series1.add(3, 3);
        series1.add(4, 4);
        series1.add(4, 5);
        
        std::vector<PCSeries> signalV;
        signalV.emplace_back(std::move(series0));
        signalV.emplace_back(std::move(series1));
        
        PCSignal signal(std::move(signalV));
        
        SignalVars vars(VARS_XN, 2);
        ExprSet s;
        PCMonitor m(signal, vars, s);
        
        PCSeries expected0;
        expected0.add(4, 0);
        expected0.add(4, 0);
        
        PCSeries expected1;
        expected1.add(0, 5);
        expected1.add(1, 4);
        expected1.add(1, 4);
        expected1.add(2, 3);
        expected1.add(2, 2);
        expected1.add(3, 1);
        expected1.add(3, 1);
        expected1.add(4, 0);
        expected1.add(4, 0);       
        
        
        const PCSeries &rev0 = m.makeReverse(static_cast<const VarExpr*>(s.makeVar("x0").get()));
        ASSERT_EQ(expected0, rev0);
        const PCSeries &rev0Copy = m.makeReverse(static_cast<const VarExpr*>(s.makeVar("x0").get()));
        ASSERT_EQ(&rev0, &rev0Copy);
        
        const PCSeries &rev1 = m.makeReverse(static_cast<const VarExpr*>(s.makeVar("x1").get()));
        ASSERT_EQ(expected1, rev1);
        const PCSeries &rev1Copy = m.makeReverse(static_cast<const VarExpr*>(s.makeVar("x1").get()));
        ASSERT_EQ(&rev1, &rev1Copy);
    }
}

TEST(MonitorTests, TestConstVarRobustness) {
    PCSeries series;
    series.add(1, 0);
    series.add(2, 1);
    series.add(3, 0);
    series.add(4, 1);
    series.add(4, 1);
    
    PCSignal signal(std::move(series));    
    SignalVars vars(VARS_XN, 1);
    
    ExprSet s;    
    PCRobMonitor monitor(signal, vars, s);
    
    {
        ExprPtr e = s.makeConst(0);
        PCSeries expected;
        expected.add(4, 0);
        expected.add(4, 0);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeConst(2);
        PCSeries expected;
        expected.add(4, 2);
        expected.add(4, 2);
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeBoolConst(false);
        PCSeries expected;
        expected.add(Time(4), Space::minusInf());
        expected.add(Time(4), Space::minusInf());
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeBoolConst(true);
        PCSeries expected;
        expected.add(Time(4), Space::inf());
        expected.add(Time(4), Space::inf());
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(expected, output);
    }
    
    {
        ExprPtr e = s.makeVar("x0");
        const PCSeries &output = monitor.makeOutput(e);
        ASSERT_EQ(signal[0], output);    
    }
}

TEST(MonitorTests, TestComparisonRobustness) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series;
        series.add(1, 0);        
        series.add(3, 1);
        series.add(3, -1);
        series.add(5, 2);
        series.add(5, -2);
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0  = s.makeApply(APPLY_GEQ, {x0, s.makeConst(0)});
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(signal[0], result0);
        
        ExprPtr e1  = s.makeApply(APPLY_GT, {x0, s.makeConst(1)});
        PCSeries expected1;
        expected1.add(1, -1);        
        expected1.add(3, 0);
        expected1.add(3, -2);
        expected1.add(5, 1);
        expected1.add(5, -3);
        const PCSeries &result1 = m.makeOutput(e1);        
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2  = s.makeApply(APPLY_LEQ, {s.makeConst(-1), s.makeConst(1)});
        PCSeries expected2;
        expected2.add(5, 2);
        expected2.add(5, 2);
        const PCSeries &result2 = m.makeOutput(e2);        
        ASSERT_EQ(expected2, result2);
    }
}

TEST(MonitorTests, TestBoolOpRobustness) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    
    {
        PCSeries series;
        series.add(1, 0);        
        series.add(3, 1);
        series.add(3, -1);
        series.add(5, 2);
        series.add(5, -2);
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0  = s.makeApply(APPLY_BOOL_OR, {s.makeApply(APPLY_GEQ, {x0, s.makeConst(0)}), s.makeApply(APPLY_LT, {x0, s.makeConst(0)}) });
        PCSeries expected0;
        expected0.add(1, 0);        
        expected0.add(3, 1);
        expected0.add(3, 1);
        expected0.add(5, 2);
        expected0.add(5, 2);
        const PCSeries &result0 = m.makeOutput(e0);        
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1  = s.makeApply(APPLY_BOOL_AND, {s.makeApply(APPLY_GEQ, {x0, s.makeConst(0)}), s.makeApply(APPLY_LT, {x0, s.makeConst(0)}) });
        PCSeries expected1;
        expected1.add(1, 0);        
        expected1.add(3, -1);
        expected1.add(3, -1);
        expected1.add(5, -2);
        expected1.add(5, -2);
        const PCSeries &result1 = m.makeOutput(e1);        
        ASSERT_EQ(expected1, result1);
    }
}

TEST(MonitorTests, TestFRobustness) {
    SignalVars vars(VARS_XN, 1);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    ExprPtr c0 = s.makeConst(0);
    
    {
        PCSeries series;        
        series.add(0, 0);
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporal(ClosedIntvl(0, 0), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(0, 0);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeTemporal(ClosedIntvl(1, 1), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(Time(0), Space::minusInf());
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeTemporal(ClosedIntvl(0, 2), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(0, 0);
        ASSERT_EQ(expected2, result2);
    }
    
    {
        PCSeries series;
        series.add(4, 3);
        series.add(8, 1);
        series.add(12, 5);
        series.add(12, 5);
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 3);
        expected0.add(7, 1);
        expected0.add(12, 5);
        expected0.add(12, 5);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeTemporal(ClosedIntvl(0, 4), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, 3);
        expected1.add(12, 5);
        expected1.add(12, 5);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeTemporal(ClosedIntvl(0, 8), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result2 = m.makeOutput(e2);
        PCSeries expected2;
        expected2.add(12, 5);
        expected2.add(12, 5);
        ASSERT_EQ(expected2, result2);
        
        ExprPtr e3 = s.makeTemporal(ClosedIntvl(0, 9), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(12, 5);
        expected3.add(12, 5);
        ASSERT_EQ(expected3, result3);
    }
    
    {
        PCSeries series;
        series.add(4, 5);
        series.add(8, 1);
        series.add(8, 1);
        series.add(12, 3);
        series.add(12, 1);
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporal(ClosedIntvl(0, 1), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 5);
        expected0.add(7, 1);
        expected0.add(7, 1);
        expected0.add(12, 3);
        expected0.add(12, 1);
        ASSERT_EQ(expected0, result0);
        
        ExprPtr e1 = s.makeTemporal(ClosedIntvl(0, 4), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result1 = m.makeOutput(e1);
        PCSeries expected1;
        expected1.add(4, 5);
        expected1.add(4, 1);
        expected1.add(12, 3);
        expected1.add(12, 1);
        ASSERT_EQ(expected1, result1);
        
        ExprPtr e2 = s.makeTemporal(ClosedIntvl(0, 8), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result22 = m.makeOutput(e2);
        PCSeries expected22;
        expected22.add(4, 5);
        expected22.add(12, 3);
        expected22.add(12, 1);
        ASSERT_EQ(expected22, result22);
        
        ExprPtr e3 = s.makeTemporal(ClosedIntvl(1, 5), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result3 = m.makeOutput(e3);
        PCSeries expected3;
        expected3.add(3, 5);
        expected3.add(3, 1);
        expected3.add(11, 3);
        expected3.add(11, 1);
        expected3.add(Time(12), Space::minusInf());
        expected3.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected3, result3);
        
        ExprPtr e4 = s.makeTemporal(ClosedIntvl(4, 4), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result4 = m.makeOutput(e4);
        PCSeries expected4;    
        expected4.add(4, 1);
        expected4.add(4, 1);
        expected4.add(8, 3);
        expected4.add(8, 1);
        expected4.add(Time(12), Space::minusInf());
        expected4.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected4, result4);
        
        ExprPtr e5 = s.makeTemporal(ClosedIntvl(8, 8), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result5 = m.makeOutput(e5);
        PCSeries expected5;    
        expected5.add(0, 1);
        expected5.add(4, 3);
        expected5.add(4, 1);
        expected5.add(Time(12), Space::minusInf());
        expected5.add(Time(12), Space::minusInf());
        ASSERT_EQ(expected5, result5);
    }
    
    {
        PCSeries series;
        series.add(4, 3);
        series.add(4, 3);
        series.add(8, 1);
        series.add(8, 1);
        series.add(12, 3); 
        PCSignal signal(std::move(series));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporal(ClosedIntvl(0, 4), TEMPORAL_F, s.makeApply(APPLY_GEQ, {x0, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(12, 3);
        ASSERT_EQ(expected0, result0);
    }
}

TEST(MonitorTests, TestUntilRobustness) {
    SignalVars vars(VARS_XN, 2);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    ExprPtr x1 = s.makeVar("x1");
    ExprPtr c0 = s.makeConst(0);
    {
        PCSeries series0;
        series0.add(0, 1);
        PCSeries series1;
        series1.add(0, 2);
        std::vector<PCSeries> seriesV;
        seriesV.emplace_back(std::move(series0));
        seriesV.emplace_back(std::move(series1));
        PCSignal signal(std::move(seriesV));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporalUntil(ClosedIntvl::zeroToInf(), s.makeApply(APPLY_GEQ, {x0, c0}), s.makeApply(APPLY_GEQ, {x1, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(0, 1);
        ASSERT_EQ(expected0, result0);
    }
    {
        PCSeries series0;
        series0.add(5, 5);
        series0.add(8, 1);
        PCSeries series1;
        series1.add(1, 0);
        series1.add(2, 1);
        series1.add(3, 0);
        series1.add(4, 3);
        series1.add(6, 0);
        series1.add(7, 5);
        series1.add(8, 0);
        std::vector<PCSeries> seriesV;
        seriesV.emplace_back(std::move(series0));
        seriesV.emplace_back(std::move(series1));
        PCSignal signal(std::move(seriesV));
        
        PCRobMonitor m(signal, vars, s);
        
        ExprPtr e0 = s.makeTemporalUntil(ClosedIntvl::zeroToInf(), s.makeApply(APPLY_GEQ, {x0, c0}), s.makeApply(APPLY_GEQ, {x1, c0}));
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 3);
        expected0.add(7, 1);
        expected0.add(8, 0);
        ASSERT_EQ(expected0, result0);
    }
}

TEST(MonitorTests, TestIteRobustness) {
    SignalVars vars(VARS_XN, 2);
    ExprSet s;
    ExprPtr x0 = s.makeVar("x0");
    ExprPtr x1 = s.makeVar("x1");
    ExprPtr c0 = s.makeConst(0);
    {
        PCSeries series0;
        series0.add(0, 1);
        PCSeries series1;
        series1.add(0, 2);
        std::vector<PCSeries> seriesV;
        seriesV.emplace_back(std::move(series0));
        seriesV.emplace_back(std::move(series1));
        PCSignal signal(std::move(seriesV));
        
        PCRobMonitor m(signal, vars, s);
        // Equivalent to the StlUntil-formula without the 'ite'.
        ExprPtr e0 = parseSExprStr(s, "(ite (not (F (0 inf) (>= x1 0))) false (StlUntil (0 inf) (>= x0 0) (>= x1 0)))");
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(0, 1);
        ASSERT_EQ(expected0, result0);
    }
    {
        PCSeries series0;
        series0.add(5, 5);
        series0.add(8, 1);
        PCSeries series1;
        series1.add(1, 0);
        series1.add(2, 1);
        series1.add(3, 0);
        series1.add(4, 3);
        series1.add(6, 0);
        series1.add(7, 5);
        series1.add(8, 0);
        std::vector<PCSeries> seriesV;
        seriesV.emplace_back(std::move(series0));
        seriesV.emplace_back(std::move(series1));
        PCSignal signal(std::move(seriesV));
        
        PCRobMonitor m(signal, vars, s);
        // Equivalent to the StlUntil-formula without the 'ite'.
        ExprPtr e0 = parseSExprStr(s, "(ite (not (F (0 inf) (>= x1 0))) false (StlUntil (0 inf) (>= x0 0) (>= x1 0)))");
        const PCSeries &result0 = m.makeOutput(e0);
        PCSeries expected0;
        expected0.add(4, 3);
        expected0.add(7, 1);
        expected0.add(8, 0);
        ASSERT_EQ(expected0, result0);
    }
}
