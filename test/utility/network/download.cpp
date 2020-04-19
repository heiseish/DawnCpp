#include "utility/network/download.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class DownloadTest : public ::testing::Test {
protected:
    virtual void SetUp() { GTEST_SKIP(); }
};

TEST_F(DownloadTest, DownloadSuccessfully) {
    EXPECT_TRUE(
        Download("https://www.w3.org/WAI/ER/tests/xhtml/testfiles/resources/"
                 "pdf/dummy.pdf",
                 "/tmp/tmp.pdf", true));
}

}  // namespace Dawn::Utility