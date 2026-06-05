#include "gtest/gtest.h"

#include "Stle/Version.hpp"

TEST(VersionTests, TestVersion) {
    ASSERT_NE(nullptr, Stle::stleVersion());
    ASSERT_NE(std::string(""), std::string(Stle::stleVersion()));
}
