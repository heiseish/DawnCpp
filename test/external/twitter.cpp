#include "external/twitter.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "gtest/gtest.h"
#include "utility/general/env.hpp"
namespace Dawn::External {

class TwitterAPITesting : public ::testing::Test {
protected:
    std::unique_ptr<TwitterAPI> _api;
    virtual void SetUp()
    {
        GTEST_SKIP();
        _api = std::make_unique<TwitterAPI>(
            Utility::SafeGetEnv("TWITTER_ACCESS_TOKEN_KEY"),
            Utility::SafeGetEnv("TWITTER_ACCESS_TOKEN_SECRET"),
            Utility::SafeGetEnv("TWITTER_CONSUMER_KEY"),
            Utility::SafeGetEnv("TWITTER_CONSUMER_SECRET"));
    }
};

TEST_F(TwitterAPITesting, GetTimelineCorrectly)
{
    auto res = _api->RecentTimelinePosts("xzhan1005");
    EXPECT_EQ(res.size(), 5u);
}

}  // namespace Dawn::External