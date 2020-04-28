#include "utility/filesystem/current_directory.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class GetCurrentDirectoryTest : public ::testing::Test {
};

TEST_F(GetCurrentDirectoryTest, GetCurrentDirectory)
{
    auto res = GetCurrentWorkingDirectory();
    EXPECT_EQ(res, "/Users/heiseish/Projects/DawnCpp/build");
}

}  // namespace Dawn::Utility