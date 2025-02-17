#include "library/algorithm/string/kmp.hpp"
#include "gtest/gtest.h"

namespace Dawn::Library::Algorithm {

class KmpAlgorithmTest : public ::testing::Test {};

TEST_F(KmpAlgorithmTest, CheckASCIIContainsCorrectly)
{
    const char* text = "have this child";
    const char* pattern = "this";
    EXPECT_TRUE(KMPAlgorithm::Contains(text, pattern));
    EXPECT_FALSE(KMPAlgorithm::Contains(text, "mama"));
    EXPECT_FALSE(KMPAlgorithm::Contains(text, "thIS"));
}

TEST_F(KmpAlgorithmTest, CheckUTF8ContainsCorrectly)
{
    const char* text = "một ngày mới";
    const char* pattern = "ngày";
    EXPECT_TRUE(KMPAlgorithm::Contains(text, pattern));
    EXPECT_FALSE(KMPAlgorithm::Contains(text, "đâu"));
    EXPECT_FALSE(KMPAlgorithm::Contains(text, "Ngày"));
}

TEST_F(KmpAlgorithmTest, CheckOccurenceIndciesCorrectly)
{
    const char* text = "news about Donald trump news";
    const char* pattern = "news";
    auto res = KMPAlgorithm::OccurrenceIndices(text, pattern);
    std::vector<unsigned int> ground_truth{0u, 24u};
    EXPECT_EQ(res, ground_truth);
}

}  // namespace Dawn::Library::Algorithm