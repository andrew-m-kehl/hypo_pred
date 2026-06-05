#include "gtest/gtest.h"

#include "CStle/cstle.h"

#include "Stle/PCSignal.hpp"
#include "Stle/ExprSet.hpp"

#include "Stle/U/File.hpp"
#include "Stle/U/FilePrinter.hpp"

using namespace Stle;

// TODO: Use stle_delete_expr.

TEST(CStleTests, TestVersion) {
    ASSERT_NE(nullptr, stle_version());
    ASSERT_NE(std::string(""), std::string(stle_version()));

    ASSERT_NE(nullptr, stle_c_api_version());
    ASSERT_NE(std::string(""), std::string(stle_c_api_version()));
}

TEST(CStleTests, TestReadSignalCsvTimeFirst) {
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(2, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(1, 3);
        expectedV[1].add(3, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::File file("tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        stle_pcsignal *result = stle_read_pcsignal_csv_file(file.file(), STLE_READ_SIGNAL_DEFAULT);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(2, 1);
        expectedV[0].add(2, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(1, 3);
        expectedV[1].add(1, 3);
        expectedV[1].add(3, 4);
        expectedV[1].add(3, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        U::File file("tests/data/SignalTests/TestReadCsvTimeFirst.csv", "r");
        stle_pcsignal *result = stle_read_pcsignal_csv_file(file.file(), STLE_READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(1, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);        
        
        expectedV[1].add(0, 3);
        expectedV[1].add(2, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));

        stle_pcsignal *result = stle_read_pcsignal_csv_fname("tests/data/SignalTests/TestReadCsvTimeFirst.csv", STLE_READ_SIGNAL_END_POINTS);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        std::vector<PCSeries> expectedV(2);
        
        expectedV[0].add(1, 1);
        expectedV[0].add(1, 1);
        expectedV[0].add(4, 2);
        expectedV[0].add(4, 2);
        
        expectedV[1].add(0, 3);        
        expectedV[1].add(2, 4);
        expectedV[1].add(2, 4);
        expectedV[1].add(4, 5);
        expectedV[1].add(4, 5);
        
        PCSignal expected(std::move(expectedV));
        
        stle_pcsignal *result = stle_read_pcsignal_csv_fname("tests/data/SignalTests/TestReadCsvTimeFirst.csv", STLE_READ_SIGNAL_END_POINTS | STLE_READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
}

TEST(CStleTests, TestReadSignalArrayTimeFirst) {
    // Data by row.
    double data01[] = { 0, 0,  1, 1,  2, 2,  3, 3 };    
    // Same data but by column.
    double data23[] = {0, 1, 2, 3,  0, 1, 2, 3};
    
    double data45_0[] = {1, 1};
    double data45_1[] = {2, 2};
    double data45_2[] = {3, 3};
    double *data45[] = {data45_0, data45_1, data45_2};
    
    {
        PCSeries expectedS;
        expectedS.add(1, 0);
        expectedS.add(2, 1);
        expectedS.add(3, 2);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));        
        
        stle_pcsignal *result = stle_read_pcsignal_array(data01, 4, 2, 1, STLE_READ_SIGNAL_DEFAULT);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }    
    {
        PCSeries expectedS;
        expectedS.add(1, 0);
        expectedS.add(1, 0);
        expectedS.add(2, 1);
        expectedS.add(2, 1);
        expectedS.add(3, 2);
        expectedS.add(3, 2);    
        PCSignal expected(std::move(expectedS));

        stle_pcsignal *result = stle_read_pcsignal_array(data01, 4, 2, 1, STLE_READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        PCSeries expectedS;
        expectedS.add(0, 0);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        stle_pcsignal *result = stle_read_pcsignal_array(data23, 4, 2, 0, STLE_READ_SIGNAL_END_POINTS);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        PCSeries expectedS;
        expectedS.add(0, 0);
        expectedS.add(1, 1);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));

        stle_pcsignal *result = stle_read_pcsignal_array(data23, 4, 2, 0, STLE_READ_SIGNAL_END_POINTS | STLE_READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        PCSeries expectedS;
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));
        
        stle_pcsignal *result = stle_read_pcsignal_array2(data45, 3, 2, STLE_READ_SIGNAL_END_POINTS);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
    {
        PCSeries expectedS;
        expectedS.add(1, 1);
        expectedS.add(1, 1);
        expectedS.add(2, 2);
        expectedS.add(2, 2);
        expectedS.add(3, 3);
        expectedS.add(3, 3);
        PCSignal expected(std::move(expectedS));        
                
        stle_pcsignal *result = stle_read_pcsignal_array2(data45, 3, 2, STLE_READ_SIGNAL_END_POINTS | STLE_READ_SIGNAL_RIGHT_CLOSED);
        ASSERT_NE(nullptr, result);
        ASSERT_EQ(expected, *reinterpret_cast<PCSignal*>(result));
        stle_delete_pcsignal(result);
        result = nullptr;
    }
}

TEST(CStleTests, TestParseSExprStr) {
    // TODO: Perhaps copy more tests from SExprParserTests::TestParseChar    
    stle_exprset *exprset = stle_make_exprset();
    ASSERT_NE(nullptr, exprset);
    ExprSet &es = *reinterpret_cast<ExprSet*>(exprset);

    ExprPtr x0 = es.makeVar("x0");
    ExprPtr x1 = es.makeVar("x1");
    ExprPtr x2 = es.makeVar("x2");
    {
        const char *str = "x0 x1";
        int pos = 0;

        const stle_expr *result0 = stle_parse_sexpr_str(exprset, str, &pos);
        ASSERT_NE(nullptr, result0);
        ASSERT_EQ(x0.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result0)));
        stle_unref_expr(result0);
        result0 = nullptr;

        str += pos;
        const stle_expr *result1 = stle_parse_sexpr_str(exprset, str, nullptr);
        ASSERT_NE(nullptr, result1);
        ASSERT_EQ(x1.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result1)));
        stle_unref_expr(result1);
        result1 = nullptr;
    }
    {
        const char *str = "(max) (max x0) (max x0 x1) (max x0 x1 x2)";
        int pos = 0;

        ExprPtr e0 = es.makeApply(APPLY_MAX, {});
        ExprPtr e1 = es.makeApply(APPLY_MAX, {x0});
        ExprPtr e2 = es.makeApply(APPLY_MAX, {x0, x1});
        ExprPtr e3 = es.makeApply(APPLY_MAX, {x0, x1, x2});

        const stle_expr *result0 = stle_parse_sexpr_str(exprset, str, &pos);
        ASSERT_NE(nullptr, result0);
        ASSERT_EQ(e0.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result0)));
        stle_unref_expr(result0);
        result0 = nullptr;

        str += pos;
        const stle_expr *result1 = stle_parse_sexpr_str(exprset, str, &pos);
        ASSERT_NE(nullptr, result1);
        ASSERT_EQ(e1.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result1)));
        stle_unref_expr(result1);
        result1 = nullptr;

        str += pos;
        const stle_expr *result2 = stle_parse_sexpr_str(exprset, str, &pos);
        ASSERT_NE(nullptr, result2);
        ASSERT_EQ(e2.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result2)));
        stle_unref_expr(result2);
        result2 = nullptr;

        str += pos;
        const stle_expr *result3 = stle_parse_sexpr_str(exprset, str, nullptr);
        ASSERT_NE(nullptr, result3);
        ASSERT_EQ(e3.get(), reinterpret_cast<const Expr*>(stle_get_expr_impl(result3)));
        stle_unref_expr(result3);
        result3 = nullptr;
    }

    // On purpose deleting the exprset before the ExprPtr-s get destructed.
    stle_delete_exprset(exprset);
    exprset = nullptr;
}

// TODO: TestParseSExprFile/FName

TEST(CStleTests, TestMonitorRequestResponse) {
    // NOTE: The test uses the functions with 'offline' in the name; there is a user who
    // uses these functions, and they should be callable.
    {
        U::File file("tests/data/MonitorTests/TestRequestResponse01.csv", "r");
        stle_pcsignal *signal = stle_read_pcsignal_csv_file(file.file(), STLE_READ_SIGNAL_DEFAULT);
        ASSERT_NE(nullptr, signal);
        stle_exprset *exprset = stle_make_exprset();
        const stle_expr *expr = stle_parse_sexpr_str(exprset, "(G (0 inf) (-> x0 (F (0 1) x1)))", nullptr);
        stle_signalvars *vars  =stle_make_signalvars_xn(stle_pcsignal_size(signal));
        stle_offlinepcmonitor *monitor = stle_make_offlinepcmonitor(signal, vars, exprset);
        const stle_pcseries *series = stle_offlinepcmonitor_make_output(monitor, expr, true, nullptr);
        double value0 = stle_pcseries_value0(series);;
        ASSERT_EQ(1, value0);

        stle_delete_offlinepcmonitor(monitor);
        monitor = nullptr;
        stle_delete_signalvars(vars);        
        vars = nullptr;
        stle_delete_pcsignal(signal);
        signal = nullptr;
        stle_unref_expr(expr);
        expr = nullptr;
        stle_delete_exprset(exprset);
        exprset = nullptr;
    }
    {
        U::File file("tests/data/MonitorTests/TestRequestResponse01.csv", "r");
        stle_pcsignal *signal = stle_read_pcsignal_csv_file(file.file(), STLE_READ_SIGNAL_DEFAULT);
        ASSERT_NE(nullptr, signal);
        stle_exprset *exprset = stle_make_exprset();
        const stle_expr *expr = stle_parse_sexpr_str(exprset, "(G (0 inf) (-> 1 x0 (F (0 0.5) x1)))", nullptr);
        stle_signalvars *vars = stle_make_signalvars_xn(stle_pcsignal_size(signal));
        stle_offlinepcmonitor *monitor = stle_make_offlinepcmonitor(signal, vars, exprset);
        const stle_pcseries *series = stle_offlinepcmonitor_make_output(monitor, expr, true, nullptr);
        double value0 = stle_pcseries_value0(series);;
        ASSERT_EQ(0, value0);        

        stle_delete_offlinepcmonitor(monitor);
        monitor = nullptr;
        stle_delete_signalvars(vars);
        vars = nullptr;
        stle_delete_pcsignal(signal);
        signal = nullptr;
        stle_unref_expr(expr);
        expr = nullptr;
        stle_delete_exprset(exprset);
        exprset = nullptr;
    }
    {
        U::File exprFile("tests/data/MonitorTests/TestRequestResponse2.stl", "r");
        stle_pcsignal *signal = stle_read_pcsignal_csv_fname("tests/data/MonitorTests/TestRequestResponse23.csv", STLE_READ_SIGNAL_DEFAULT);
        ASSERT_NE(nullptr, signal);
        stle_exprset *exprset = stle_make_exprset();
        const stle_expr *expr = stle_parse_sexpr_file(exprset, exprFile.file());
        stle_signalvars *vars  =stle_make_signalvars_xn(stle_pcsignal_size(signal));
        stle_offlinepcmonitor *monitor = stle_make_offlinepcmonitor(signal, vars, exprset);
        const stle_pcseries *series = stle_offlinepcmonitor_make_output(monitor, expr, true, nullptr);
        double value0 = stle_pcseries_value0(series);;
        ASSERT_EQ(1, value0);        

        stle_delete_offlinepcmonitor(monitor);
        monitor = nullptr;
        stle_delete_signalvars(vars);
        vars = nullptr;
        stle_delete_pcsignal(signal);
        signal = nullptr;
        stle_unref_expr(expr);
        expr = nullptr;
        stle_delete_exprset(exprset);
        exprset = nullptr;
    }
    {
        U::File exprFile("tests/data/MonitorTests/TestRequestResponse3.stl", "r");
        stle_pcsignal *signal = stle_read_pcsignal_csv_fname("tests/data/MonitorTests/TestRequestResponse23.csv", STLE_READ_SIGNAL_DEFAULT);
        stle_exprset *exprset = stle_make_exprset();
        const stle_expr *expr = stle_parse_sexpr_file(exprset, exprFile.file());
        stle_signalvars *vars = stle_make_signalvars_xn(stle_pcsignal_size(signal));
        stle_offlinepcmonitor *monitor = stle_make_offlinepcmonitor(signal, vars, exprset);
        const stle_pcseries *series = stle_offlinepcmonitor_make_output(monitor, expr, true, nullptr);
        double value0 = stle_pcseries_value0(series);;
        ASSERT_EQ(0, value0);        

        stle_delete_offlinepcmonitor(monitor);
        monitor = nullptr;
        stle_delete_signalvars(vars);
        vars = nullptr;
        stle_delete_pcsignal(signal);
        signal = nullptr;
        stle_unref_expr(expr);
        expr = nullptr;
        stle_delete_exprset(exprset);
        exprset = nullptr;
    }
}

TEST(CStleTests, TestRobustnessMonitor) {
    U::File file("tests/data/MonitorTests/TestRequestResponseRobustness.csv", "r");
    stle_pcsignal *signal = stle_read_pcsignal_csv_file(file.file(), STLE_READ_SIGNAL_END_POINTS);    
    ASSERT_NE(nullptr, signal);
    
    stle_exprset *exprset = stle_make_exprset();
    const stle_expr *expr = stle_parse_sexpr_str(exprset, "(-> (>= x0 4.5) (F (0 2) (>= x1 4.5)))", nullptr);    
    
    stle_signalvars *vars = stle_make_signalvars_xn(stle_pcsignal_size(signal));
    stle_pcrobmonitor *monitor = stle_make_pcrobmonitor(signal, vars, exprset);
    
    const stle_pcseries *result = stle_pcrobmonitor_make_output(monitor, expr, true, nullptr);    
    stle_pcseries *expected = stle_make_pcseries();
    stle_pcseries_add(expected, 4, 1);
    stle_pcseries_add(expected, 9, 4.5);
    stle_pcseries_add(expected, 11, 1.5);
    stle_pcseries_add(expected, 15, 4.5);
    stle_pcseries_add(expected, 17, -0.5);
    stle_pcseries_add(expected, 20, 4.5);
    stle_pcseries_add(expected, 20, 4.5);
    
    ASSERT_TRUE(stle_pcseries_equal(expected, result));
    
    stle_delete_pcseries(expected);
    expected = nullptr;
    stle_delete_pcrobmonitor(monitor);
    monitor = nullptr;
    stle_delete_signalvars(vars);
    vars = nullptr;    
    stle_unref_expr(expr);
    expr = nullptr;
    stle_delete_exprset(exprset);
    exprset = nullptr;
    stle_delete_pcsignal(signal);
    signal = nullptr;
}
