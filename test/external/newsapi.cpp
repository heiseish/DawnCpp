#include "external/newsapi.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "gtest/gtest.h"
namespace Dawn::External {

class NewsAPITest : public ::testing::Test {
protected:
    std::unique_ptr<NewsAPI> _api;
    virtual void SetUp()
    {
        GTEST_SKIP();
        _api = std::make_unique<NewsAPI>(std::getenv("NEWSAPI_KEY"));
    }
};

TEST_F(NewsAPITest, GetTopHeadlinesWithoutQuery)
{
    auto res = _api->TopHeadlines("", 1u);
    EXPECT_EQ(res.size(), 1u);
}

TEST_F(NewsAPITest, GetTopHeadlinesWithQuery)
{
    auto res = _api->TopHeadlines("Donald Trump", 5u);
    EXPECT_EQ(res.size(), 5u);
}

}  // namespace Dawn::External