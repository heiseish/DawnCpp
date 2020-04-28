#pragma once

#ifndef DAWN_TIMER_H
#define DAWN_TIMER_H

#include <sys/time.h>
#include <chrono>

using MilliSeconds = std::chrono::milliseconds;
using MicroSeconds = std::chrono::microseconds;
using NanoSeconds = std::chrono::nanoseconds;
using Seconds = std::chrono::seconds;

namespace Dawn::Utility {

time_t GetCurrentEpoch();
unsigned long GetCurrentEpochMs();

class Timer {
public:
    Timer() {}
    void Start();

    template <typename unit = MicroSeconds>
    decltype(auto) Record()
    {
        auto end_ = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<unit>(end_ - _start).count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

}  // namespace Dawn::Utility

#endif