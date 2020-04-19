#include "utility/filesystem/join.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class JoinTest : public ::testing::Test {};

TEST_F(JoinTest, JoinCorrectly) {
    std::string base = "/Users/heiseish/";
    auto res = PathJoin(base, "Projects", "DawnCpp/build");
    EXPECT_EQ(res, "/Users/heiseish/Projects/DawnCpp/build");
}

}  // namespace Dawn::Utility