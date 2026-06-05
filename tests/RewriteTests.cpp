#include "gtest/gtest.h"

#include "Stle/ExprSet.hpp"
#include "Stle/PCRewriter.hpp"
#include "Stle/SExprParser.hpp"

using namespace Stle;

TEST(RewriteTests, TestRewriteOnTemporal) {
    ExprSet s;
    
    {
        ExprPtr expr4 = parseSExprStr(s, "(< (On (0 200) (- (Max x0) (Min x0))) 0.01)");
        ExprPtr expected4 = parseSExprStr(s, "(< (- (On (0 200) (Max x0)) (On (0 200) (Min x0))) 0.01)");
        ExprPtr rewritten4 = rewriteForPCMonitor(s, expr4);
        ASSERT_EQ(expected4, rewritten4);
    }   
    {
        ExprPtr expr5 = parseSExprStr(s, "(G (0 inf) (F (0 inf) (< (On (0 200) (- (Max x0) (Min x0))) 0.01)))");
        ExprPtr expected5 = parseSExprStr(s, "(G (0 inf) (F (0 inf) (< (- (On (0 200) (Max x0)) (On (0 200) (Min x0))) 0.01)))");
        ExprPtr rewritten5 = rewriteForPCMonitor(s, expr5);
        ASSERT_EQ(expected5, rewritten5);
    }
    {
        ExprPtr expr6 = parseSExprStr(s, "(On (0 inf) (+ (- (Min x0)) (- (Max x1))))");
        ExprPtr expected6 = parseSExprStr(s, "(+ (- (On (0 inf) (Min x0))) (- (On (0 inf) (Max x1))))");
        ExprPtr rewritten6 = rewriteForPCMonitor(s, expr6);
        ASSERT_EQ(expected6, rewritten6);
    }    
    {
        ExprPtr expr7 = parseSExprStr(s, "(F (0 inf) (On (0 10) (+ (- (Min x0)) (- (Max x1)))))");
        ExprPtr expected7 = parseSExprStr(s, "(F (0 inf) (+ (- (On (0 10) (Min x0))) (- (On (0 10) (Max x1)))))");
        ExprPtr rewritten7 = rewriteForPCMonitor(s, expr7);
        ASSERT_EQ(expected7, rewritten7);
    }    
}

TEST(RewriteTests, TestRewritePastOnTemporal) {
    ExprSet s;
    {
        ExprPtr expr0 = parseSExprStr(s, "(On (-10 10) (- (Max x0) (Min x0)))");
        ExprPtr expected0 = parseSExprStr(s, "(- (max (On (-10 0) (Max x0)) (On (0 10) (Max x0))) (min (On (-10 0) (Min x0)) (On (0 10) (Min x0))))");
        ExprPtr rewritten0 = rewriteForPCMonitor(s, expr0);
        // NOTE: Relying here on the particular order in which the rewriter puts the arguments of min and max.
        ASSERT_EQ(expected0, rewritten0);
    }
    {
        ExprPtr expr4 = parseSExprStr(s, "(< (On (-200 0) (- (Max x0) (Min x0))) 0.01)");
        ExprPtr expected4 = parseSExprStr(s, "(< (- (On (-200 0) (Max x0)) (On (-200 0) (Min x0))) 0.01)");
        ExprPtr rewritten4 = rewriteForPCMonitor(s, expr4);
        ASSERT_EQ(expected4, rewritten4);
    }
    {
        ExprPtr expr5 = parseSExprStr(s, "(G (-inf 0) (F (-inf 0) (< (On (-200 0) (- (Max x0) (Min x0))) 0.01)))");
        ExprPtr expected5 = parseSExprStr(s, "(G (-inf 0) (F (-inf 0) (< (- (On (-200 0) (Max x0)) (On (-200 0) (Min x0))) 0.01)))");
        ExprPtr rewritten5 = rewriteForPCMonitor(s, expr5);
        ASSERT_EQ(expected5, rewritten5);
    }
    {
        ExprPtr expr6 = parseSExprStr(s, "(On (-inf 0) (+ (- (Min x0)) (- (Max x1))))");
        ExprPtr expected6 = parseSExprStr(s, "(+ (- (On (-inf 0) (Min x0))) (- (On (-inf 0) (Max x1))))");
        ExprPtr rewritten6 = rewriteForPCMonitor(s, expr6);
        ASSERT_EQ(expected6, rewritten6);
    }    
    {
        ExprPtr expr7 = parseSExprStr(s, "(F (-inf 0) (On (-10 0) (+ (- (Min x0)) (- (Max x1)))))");
        ExprPtr expected7 = parseSExprStr(s, "(F (-inf 0) (+ (- (On (-10 0) (Min x0))) (- (On (-10 0) (Max x1)))))");
        ExprPtr rewritten7 = rewriteForPCMonitor(s, expr7);
        ASSERT_EQ(expected7, rewritten7);
    }
    {
        // Inner On needs to be rewritten, while the outer On does not.
        ExprPtr expr = parseSExprStr(s, "(On (0 inf) (Max (On (-1 1) (Max x0))))");
        ExprPtr expected = parseSExprStr(s, "(On (0 inf) (Max (max (On (-1 0) (Max x0)) (On (0 1) (Max x0)))))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
}

TEST(RewriteTests, TestRewriteUntil) {
    ExprSet s;
    {
        ExprPtr expr = parseSExprStr(s, "(F (-1 1) x0)");
        ExprPtr expected = parseSExprStr(s, "(or (F (-1 0) x0) (F (0 1) x0))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(G (-1 1) x0)");
        ExprPtr expected = parseSExprStr(s, "(and (G (-1 0) x0) (G (0 1) x0))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr0 = parseSExprStr(s, "(Until (1 10) inf (Get x0) x1)");
        ExprPtr expected0 = parseSExprStr(s, "(ite (not (F (1 10) x1)) inf (Lookup 1 inf (Until (0 inf) inf (Get x0) x1)))");
        ExprPtr rewritten0 = rewriteForPCMonitor(s, expr0);
        ASSERT_EQ(expected0, rewritten0);
    }
    {
        ExprPtr expr1 = parseSExprStr(s, "(Until (0 10) inf (Get x0) x1)");
        ExprPtr expected1 = parseSExprStr(s, "(ite (not (F (0 10) x1)) inf (Until (0 inf) inf (Get x0) x1))");
        ExprPtr rewritten1 = rewriteForPCMonitor(s, expr1);
        ASSERT_EQ(expected1, rewritten1);
    }
    {
        ExprPtr expr2 = parseSExprStr(s, "(Until (1 10) inf (Min x0) x1)");
        ExprPtr expected2 = parseSExprStr(s, "(ite (not (F (1 10) x1)) inf (On (0 1) (Min (Until (0 inf) inf (Min x0) x1))))");
        ExprPtr rewritten2 = rewriteForPCMonitor(s, expr2);
        ASSERT_EQ(expected2, rewritten2);
    }
    {
        ExprPtr expr3 = parseSExprStr(s, "(Until (0 10) inf (Max x0) x1)");
        ExprPtr expected3 = parseSExprStr(s, "(ite (not (F (0 10) x1)) inf (Until (0 inf) inf (Max x0) x1))");
        ExprPtr rewritten3 = rewriteForPCMonitor(s, expr3);
        ASSERT_EQ(expected3, rewritten3);
    }     
    {
        ExprPtr expr8 = parseSExprStr(s, "(Until (0 inf) 0 (+ (- (Min x0)) (- (Max x1))) x2)");
        ExprPtr expected8 = parseSExprStr(s, "(+ (- (Until (0 inf) 0 (Min x0) x2)) (- (Until (0 inf) 0 (Max x1) x2)))");
        ExprPtr rewritten8 = rewriteForPCMonitor(s, expr8);
        ASSERT_EQ(expected8, rewritten8);
    }
    {
        ExprPtr expr9 = parseSExprStr(s, "(Until (0 inf) 0 (Get x2) (On (0 inf) (+ (- (Min x0)) (- (Max x1)))))");
        ExprPtr expected9 = parseSExprStr(s, "(Until (0 inf) 0 (Get x2) (+ (- (On (0 inf) (Min x0))) (- (On (0 inf) (Max x1)))))");
        ExprPtr rewritten9 = rewriteForPCMonitor(s, expr9);
        ASSERT_EQ(expected9, rewritten9);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (0 inf) (>= x0 0) (>= x1 0))");
        ExprPtr expected = parseSExprStr(s, "(StlUntil (0 inf) (>= x0 0) (>= x1 0))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (0 inf) (>= x0 0) (G (-1 1) x0))");
        ExprPtr expected = parseSExprStr(s, "(StlUntil (0 inf) (>= x0 0) (and (G (-1 0) x0) (G (0 1) x0)))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (1 2) (>= x0 0) (>= x1 0))");
        ExprPtr expected = parseSExprStr(s, "(ite (not (F (1 2) (>= x1 0))) false (G (0 1) (StlUntil (0 inf) (>= x0 0) (>= x1 0))))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
}

TEST(RewriteTests, TestRewritePastUntil) {
    ExprSet s;
    {
        ExprPtr expr0 = parseSExprStr(s, "(Until (-10 -1) inf (Get x0) x1)");
        ExprPtr expected0 = parseSExprStr(s, "(ite (not (F (-10 -1) x1)) inf (Lookup -1 inf (Until (-inf 0) inf (Get x0) x1)))");
        ExprPtr rewritten0 = rewriteForPCMonitor(s, expr0);
        ASSERT_EQ(expected0, rewritten0);
    }
    {
        ExprPtr expr1 = parseSExprStr(s, "(Until (-10 0) inf (Get x0) x1)");
        ExprPtr expected1 = parseSExprStr(s, "(ite (not (F (-10 0) x1)) inf (Until (-inf 0) inf (Get x0) x1))");
        ExprPtr rewritten1 = rewriteForPCMonitor(s, expr1);
        ASSERT_EQ(expected1, rewritten1);
    }
    {
        ExprPtr expr2 = parseSExprStr(s, "(Until (-10 -1) inf (Min x0) x1)");
        ExprPtr expected2 = parseSExprStr(s, "(ite (not (F (-10 -1) x1)) inf (On (-1 0) (Min (Until (-inf 0) inf (Min x0) x1))))");
        ExprPtr rewritten2 = rewriteForPCMonitor(s, expr2);
        ASSERT_EQ(expected2, rewritten2);
    }
    {
        ExprPtr expr3 = parseSExprStr(s, "(Until (-10 0) inf (Max x0) x1)");
        ExprPtr expected3 = parseSExprStr(s, "(ite (not (F (-10 0) x1)) inf (Until (-inf 0) inf (Max x0) x1))");
        ExprPtr rewritten3 = rewriteForPCMonitor(s, expr3);
        ASSERT_EQ(expected3, rewritten3);
    }    
    {
        ExprPtr expr8 = parseSExprStr(s, "(Until (-inf 0) 0 (+ (- (Min x0)) (- (Max x1))) x2)");
        ExprPtr expected8 = parseSExprStr(s, "(+ (- (Until (-inf 0) 0 (Min x0) x2)) (- (Until (-inf 0) 0 (Max x1) x2)))");
        ExprPtr rewritten8 = rewriteForPCMonitor(s, expr8);
        ASSERT_EQ(expected8, rewritten8);
    }
    {
        ExprPtr expr9 = parseSExprStr(s, "(Until (-inf 0) 0 (Get x2) (On (-inf 0) (+ (- (Min x0)) (- (Max x1)))))");
        ExprPtr expected9 = parseSExprStr(s, "(Until (-inf 0) 0 (Get x2) (+ (- (On (-inf 0) (Min x0))) (- (On (-inf 0) (Max x1)))))");
        ExprPtr rewritten9 = rewriteForPCMonitor(s, expr9);
        ASSERT_EQ(expected9, rewritten9);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (-inf 0) (>= x0 0) (>= x1 0))");
        ExprPtr expected = parseSExprStr(s, "(StlUntil (-inf 0) (>= x0 0) (>= x1 0))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (-inf 0) (>= x0 0) (G (-1 1) x0))");
        ExprPtr expected = parseSExprStr(s, "(StlUntil (-inf 0) (>= x0 0) (and (G (-1 0) x0) (G (0 1) x0)))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
    {
        ExprPtr expr = parseSExprStr(s, "(StlUntil (-2 -1) (>= x0 0) (>= x1 0))");
        ExprPtr expected = parseSExprStr(s, "(ite (not (F (-2 -1) (>= x1 0))) false (G (-1 0) (StlUntil (-inf 0) (>= x0 0) (>= x1 0))))");
        ExprPtr rewritten = rewriteForPCMonitor(s, expr);
        ASSERT_EQ(expected, rewritten);
    }
}
