#include "utility/general/timer.hpp"
#include <algorithm>
#include <chrono>
#include <thread>

#include "gtest/gtest.h"
namespace Dawn::Utility {

class TimerTest : public ::testing::Test {
protected:
    Timer _timer;
};

TEST_F(TimerTest, GetPrecisionDuration) {
    _timer.Start();
    auto elapsed = _timer.Record<MilliSeconds>();
    EXPECT_EQ(elapsed, 0LL);

    _timer.Start();
    elapsed = _timer.Record<MicroSeconds>();
    EXPECT_EQ(elapsed, 0LL);

    // should be greater than 0 nanosecs
    _timer.Start();
    elapsed = _timer.Record<NanoSeconds>();
    EXPECT_GT(elapsed, 0LL);
}

TEST_F(TimerTest, GetMilliSecondsCorrectly) {
    _timer.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    int64_t elapsed = _timer.Record<MilliSeconds>();
    EXPECT_LT(std::abs(elapsed - 200) * 1.0,
              std::max<int64_t>(elapsed, 200LL) * 5 / 100);
}

TEST_F(TimerTest, GetMicroSecondsCorrectly) {
    _timer.Start();
    std::this_thread::sleep_for(std::chrono::microseconds(200));
    int64_t elapsed = _timer.Record();
    EXPECT_LT(std::abs(elapsed - 200) * 1.0,
              std::max<int64_t>(elapsed, 200LL) * 50 / 100);
}

}  // namespace Dawn::Utility