#pragma once

#ifndef DAWN_RETRY_H
#define DAWN_RETRY_H

#include "utility/general/logging.hpp"

namespace Dawn::Utility {

/**
 * @brief Call retry on delegate `retry_times` number of times
 * The call is considered successful when operator bool() on `callback(func())`
 * return true
 * @arg func Function to retry
 * @arg callback Callback function to verify whether the function is
 * successfully ran
 * @throw std::runtime_error if function failed after `retry_times` number of
 * times
 * @returns value that `func` returns
 */
template <class function_type, class callback_func>
decltype(auto) Retry(function_type func,
                     callback_func callback,
                     const unsigned int& retry_times = 7)
{
    for (unsigned int _ = 0; _ < retry_times; ++_) {
        decltype(auto) res = func();
        if (callback(res)) {
            return res;
        }
    }
    throw std::runtime_error("Retry function failed");
}

}  // namespace Dawn::Utility

#endif