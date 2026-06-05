#include "gtest/gtest.h"

#include "Stle/Cmd/PCState.hpp"
#include "Stle/Cmd/CmdRunner.hpp"
#include "Stle/Cmd/CmdRunnerVisitor.hpp"
#include "Stle/Cmd/StringResult.hpp"

#include "Stle/U/StrPrinter.hpp"

using namespace Stle;

TEST(CmdTests, TestParseCmd) {
    {
        ExprSet es;
        Cmd::SCmdParser<U::SLexer<U::StrReader>> parser(es, U::OF_READER, "(read-signal-csv \"abc\")(read-signal-csv \"def\")");
        std::unique_ptr<Cmd::Cmd> cmd;
        cmd = parser.parse();
        ASSERT_EQ(*cmd, Cmd::ReadSignalCsvCmd("abc"));
        cmd = parser.parse();
        ASSERT_EQ(*cmd, Cmd::ReadSignalCsvCmd("def"));
        cmd = parser.parse();
        ASSERT_EQ(*cmd, Cmd::EofCmd());
    }
    {
        ExprSet es;
        Cmd::SCmdParser<U::SLexer<U::StrReader>> parser(es, U::OF_READER, "(eval 1.0)(eval (F (0 inf) (> x0 0)))");
        std::unique_ptr<Cmd::Cmd> cmd;
        
        Cmd::EvalCmd expected1(es.makeConst(1));
        cmd = parser.parse();
        ASSERT_EQ(*cmd, expected1);
        
        Cmd::EvalCmd expected2(parseSExprStr(es, "(F (0 inf) (> x0 0))"));
        cmd = parser.parse();
        ASSERT_EQ(*cmd, expected2);
        
        cmd = parser.parse();
        ASSERT_EQ(*cmd, Cmd::EofCmd());
    }
}

TEST(CmdTests, TestPCStateEvalFile) {
    {
        Cmd::PCState state;
        state.readSignalFile("tests/data/MonitorTests/TestRequestResponse01.csv");
        state.eval(parseSExprStr(state.exprSet(), "(G (0 inf) (-> x0 (F (0 1) x1)))"));
        ASSERT_EQ(Cmd::SpaceResult(1), state.result());
    }
    {
        Cmd::PCState state;
        state.readSignalFile("tests/data/MonitorTests/TestRequestResponse01.csv");
        state.eval(parseSExprStr(state.exprSet(), "(G (0 inf) (-> 1 x0 (F (0 5e-1) x1)))"));
        ASSERT_EQ(Cmd::SpaceResult(0), state.result());
    }
    {
        Cmd::PCState state;
        state.readSignalFile("tests/data/MonitorTests/TestRequestResponse23.csv");
        state.evalFile("tests/data/MonitorTests/TestRequestResponse2.stl");
        ASSERT_EQ(Cmd::SpaceResult(1), state.result());
    }
    {
        Cmd::PCState state;
        state.readSignalFile("tests/data/MonitorTests/TestRequestResponse23.csv");
        state.evalFile("tests/data/MonitorTests/TestRequestResponse3.stl");
        ASSERT_EQ(Cmd::SpaceResult(0), state.result());
    }
}

TEST(CmdTests, TestUnknownVersion) {
    Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
    r.runStr("(version)");
    ASSERT_EQ(r.result(), Cmd::StringResult("unknown")); 
    ASSERT_EQ("\"unknown\"\n", std::string(r.printer().str()));
}

TEST(CmdTests, TestEvalFile) {
    {
        Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
        r.runStr("(read-signal-csv \"tests/data/MonitorTests/TestRequestResponse01.csv\") (eval (G (0 inf) (-> x0 (F (0 1) x1))))");
        ASSERT_EQ("ok\n1\n", std::string(r.printer().str()));
        ASSERT_EQ(r.result(), Cmd::SpaceResult(1));         
    }
    {
        Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
        r.runStr("(read-signal-csv \"tests/data/MonitorTests/TestRequestResponse01.csv\") (eval (G (0 inf) (-> 1 x0 (F (0 5e-1) x1))))");
        ASSERT_EQ("ok\n0\n", std::string(r.printer().str()));
        ASSERT_EQ(r.result(), Cmd::SpaceResult(0));        
    }
    {
        Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
        r.runStr("(read-signal-csv \"tests/data/MonitorTests/TestRequestResponse23.csv\") (eval-file \"tests/data/MonitorTests/TestRequestResponse2.stl\")");
        ASSERT_EQ("ok\n1\n", std::string(r.printer().str()));
        ASSERT_EQ(r.result(), Cmd::SpaceResult(1));        
    }
    {
        Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
        r.runStr("(read-signal-csv \"tests/data/MonitorTests/TestRequestResponse23.csv\") (eval-file \"tests/data/MonitorTests/TestRequestResponse3.stl\")");
        ASSERT_EQ("ok\n0\n", std::string(r.printer().str()));
        ASSERT_EQ(r.result(), Cmd::SpaceResult(0));        
    }
}

TEST(CmdTests, TestEvalFileR) {
    {
        Cmd::CmdRunner<Cmd::PCState, Cmd::RunCmdVisitor, U::StrPrinter> r(U::OF_PRINTER);
        r.runStr("(set-read-signal-flags e)(read-signal-csv \"tests/data/MonitorTests/TestRequestResponseRobustness.csv\")(eval-r (-> (>= x0 4.5) (F (0 2) (>= x1 4.5))))");
        ASSERT_EQ("ok\nok\n1\n", std::string(r.printer().str()));
        ASSERT_EQ(r.result(), Cmd::SpaceResult(1));        
    }
}

// TODO: Test clear monitor
