#include "utility/string/split.hpp"
#include <string>
#include <vector>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class StringSplitTest : public ::testing::Test {
};

TEST_F(StringSplitTest, SplitCorrectly)
{
    std::string test_str = "news vietnam news about me";
    std::vector<std::string> res = Split(test_str, ' ');
    std::vector<std::string> cmp{"news", "vietnam", "news", "about", "me"};
    EXPECT_EQ(res, cmp);
}

TEST_F(StringSplitTest, SplitCommaCorrectly)
{
    std::string test_str = "news,vietnam,news,about,me";
    std::vector<std::string> res = Split(test_str, ",");
    std::vector<std::string> cmp{"news", "vietnam", "news", "about", "me"};
    EXPECT_EQ(res, cmp);
}

TEST_F(StringSplitTest, SplitStringCorrectly)
{
    std::string test_str = "hoolahupholla";
    std::vector<std::string> res = Split(test_str, "hup");
    std::vector<std::string> cmp{"hoola", "holla"};
    EXPECT_EQ(res, cmp);
}

}  // namespace Dawn::Utility