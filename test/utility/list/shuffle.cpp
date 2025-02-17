#include "utility/list/shuffle.hpp"
#include <vector>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class ShuffleTest : public ::testing::Test {};

TEST_F(ShuffleTest, ShuffleWholeVector)
{
    std::vector<int> test_vec(1000);
    for (unsigned int i = 0; i < test_vec.size(); ++i) {
        test_vec.at(i) = i;
    }
    decltype(auto) copy_vec = test_vec;
    Shuffle(copy_vec);
    EXPECT_NE(test_vec,
              copy_vec);  // chance of completely the same is very small
}

TEST_F(ShuffleTest, ShuffleHalfVector)
{
    std::vector<int> test_vec(1000);
    for (unsigned int i = 0; i < test_vec.size(); ++i) {
        test_vec.at(i) = i;
    }
    std::vector<int> copy_vec = test_vec;
    Shuffle<int>(copy_vec.begin(), copy_vec.begin() + 500);
    std::vector<int> first_half_1(copy_vec.begin(), copy_vec.begin() + 500);
    std::vector<int> first_half_2(test_vec.begin(), test_vec.begin() + 500);
    EXPECT_NE(first_half_1, first_half_2);
    std::vector<int> second_half_1(copy_vec.begin() + 500, copy_vec.end());
    std::vector<int> second_half_2(test_vec.begin() + 500, test_vec.end());
    EXPECT_EQ(second_half_1, second_half_2);
}

}  // namespace Dawn::Utility