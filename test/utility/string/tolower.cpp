#include "utility/string/tolower.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class StringTolowerTest : public ::testing::Test {
};

TEST_F(StringTolowerTest, ConvertLowerWithString)
{
    std::string test_str = "To Be Or not tO BE";
    ToLower(test_str);
    EXPECT_EQ(test_str, "to be or not to be");
}

TEST_F(StringTolowerTest, ConvertLowerWithStringIterator)
{
    std::string test_str = "To Be Or not tO BE";
    ToLower(test_str.begin(), test_str.end());
    EXPECT_EQ(test_str, "to be or not to be");
}

}  // namespace Dawn::Utility