#include "utility/string/replace.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class StringReplaceTest : public ::testing::Test {};

TEST_F(StringReplaceTest, ReplaceWithDefaultOffsset)
{
    std::string test_str = "news vietnam news about me";
    auto res = Replace(test_str, "news", "hello");
    EXPECT_EQ(res, "hello vietnam news about me");
}

TEST_F(StringReplaceTest, ReplaceWithOffsset)
{
    std::string test_str = "news vietnam news about me";
    auto res = Replace(test_str, "news", "hello", 1);
    EXPECT_EQ(res, "news vietnam hello about me");
}

TEST_F(StringReplaceTest, ReplaceWithOffsset2)
{
    std::string test_str = "aaaaa";
    auto res = Replace(test_str, "a", "b", 4);
    EXPECT_EQ(res, "aaaab");
}

TEST_F(StringReplaceTest, ReplaceAllWithEmptyStr)
{
    std::string test_str = "news vietnam news about me";
    auto res = ReplaceAll(test_str, "news");
    EXPECT_EQ(res, " vietnam  about me");
}

TEST_F(StringReplaceTest, ReplaceAllWithNonEmptyStr)
{
    std::string test_str = "news vietnam news about me";
    auto res = ReplaceAll(test_str, "news", "newtons");
    EXPECT_EQ(res, "newtons vietnam newtons about me");
}

}  // namespace Dawn::Utility