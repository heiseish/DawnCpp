#include "external/forecast.hpp"
#include <iostream>
#include <memory>
#include <string>
#include "gtest/gtest.h"
namespace Dawn::External {

class ForecastAPITesting : public ::testing::Test {
protected:
    std::unique_ptr<ForecastAPI> _api;
    virtual void SetUp() {
        GTEST_SKIP();
        _api = std::make_unique<ForecastAPI>(std::getenv("DARKSKY_KEY"));
    }
};

TEST_F(ForecastAPITesting, GetWeatherCorrectly) {
    auto res = _api->CurrentWeather();
    EXPECT_GT(res.current.size(), 0u);
    EXPECT_GT(res.summary.size(), 0u);
}

}  // namespace Dawn::External