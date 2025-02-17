#include "utility/string/trim.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class StringTrimTest : public ::testing::Test {};

TEST_F(StringTrimTest, Trim)
{
    std::string test_str = "   i    am damned   ";
    Trim(test_str);
    EXPECT_EQ(test_str, "i am damned");
}

TEST_F(StringTrimTest, TrimLeading)
{
    std::string test_str = "   i    am damned";
    TrimLeading(test_str);
    EXPECT_EQ(test_str, "i    am damned");
}

TEST_F(StringTrimTest, TrimTrailing)
{
    std::string test_str = "   i    am damned   ";
    TrimTrailing(test_str);
    EXPECT_EQ(test_str, "   i    am damned");
}

TEST_F(StringTrimTest, TrimExtraSpaces)
{
    std::string test_str = "   i    am damned   ";
    TrimExtraSpaces(test_str);
    EXPECT_EQ(test_str, " i am damned ");
}

}  // namespace Dawn::Utility