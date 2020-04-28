#include "utility/network/url.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class URLTest : public ::testing::Test {
protected:
    virtual void SetUp() {}
};

TEST_F(URLTest, EncodeSuccessfully)
{
    std::string raw_uri = "www.example.com/data?name=John Robinson";
    URLParser::EncodeURL(raw_uri);
    EXPECT_EQ(raw_uri, "www.example.com/data?name=John%20Robinson");
}

TEST_F(URLTest, RedirectSuccessfully)
{
    GTEST_SKIP();
    std::string uri = "https://source.unsplash.com/1600x900/?beach";
    auto res = URLParser::GetFinalRedirectedUrl(uri);
    EXPECT_GT(res.size(), 0u);
    EXPECT_NE(res, uri);
}

}  // namespace Dawn::Utility