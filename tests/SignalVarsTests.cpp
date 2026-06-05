#include "gtest/gtest.h"

#include "Stle/SignalVars.hpp"

using namespace Stle;

TEST(SignalVarsTests, TestMakeXN) {
    SignalVars map = SignalVars(VARS_XN, 2);
    ASSERT_EQ(0, map["x0"]);
    ASSERT_EQ(1, map["x1"]);
}
