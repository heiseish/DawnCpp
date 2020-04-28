#include "timer.hpp"
#include <chrono>

namespace Dawn::Utility {

//---------------------------------
time_t GetCurrentEpoch()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec;
}

//---------------------------------
unsigned long GetCurrentEpochMs()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void Timer::Start() { _start = std::chrono::high_resolution_clock::now(); }

}  // namespace Dawn::Utility