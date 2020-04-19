#include "utility/network/request.hpp"
#include <string>
#include "gtest/gtest.h"
namespace Dawn::Utility {

class NetworkRequestTest : public ::testing::Test {
protected:
    Utility::Requester _request;
    virtual void SetUp() { GTEST_SKIP(); }
};

TEST_F(NetworkRequestTest, GetRequestSuccessfully) {
    std::string uri = "http://date.jsontest.com/";
    auto res = _request.GetRequest(uri);
    EXPECT_TRUE(res);
    uri = "http://ip.jsontest.com/";
    res = _request.GetRequest(uri);
    EXPECT_TRUE(res);
}

TEST_F(NetworkRequestTest, PostRequestSuccessfully) {
    std::string uri = "http://date.jsontest.com/";
    auto res = _request.GetRequest(uri);
    EXPECT_TRUE(res);
    uri = "http://ip.jsontest.com/";
    res = _request.GetRequest(uri);
    EXPECT_TRUE(res);
}

}  // namespace Dawn::Utility