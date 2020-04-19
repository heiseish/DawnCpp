#include "external/unsplash.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "gtest/gtest.h"
namespace Dawn::External {

class UnsplashAPITesting : public ::testing::Test {
protected:
    std::unique_ptr<UnsplashAPI> _api;
    virtual void SetUp() {
        GTEST_SKIP();
        _api = std::make_unique<UnsplashAPI>();
    }
};

TEST_F(UnsplashAPITesting, GetUnsplashImage) {
    auto res = _api->GetRandomUnsplashPhoto();
    EXPECT_GT(res.size(), 0u);
}

}  // namespace Dawn::External