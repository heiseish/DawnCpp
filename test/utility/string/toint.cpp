#include "utility/string/toint.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class ToIntTest : public ::testing::Test {};

TEST_F(ToIntTest, ToInt) {
    const std::string test_str = "12049102";
    auto res = ToInt(test_str);
    EXPECT_EQ(res, 12049102);
    const std::string test_str2 = "1293481094091210";
    try {
        ToInt(test_str2);
        FAIL() << "Expected std::out_of_range";
    } catch (std::out_of_range const& err) {
        EXPECT_EQ(err.what(), std::string("stoi: out of range"));
    } catch (...) {
        FAIL() << "Expected std::out_of_range";
    }
}

TEST_F(ToIntTest, ToInt64) {
    std::string test_str = "1293481094091210";
    auto res = ToInt64(test_str);
    EXPECT_EQ(res, 1293481094091210LL);
}

}  // namespace Dawn::Utility