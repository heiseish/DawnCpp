#pragma once

#ifndef DAWN_UTIL_STRING_SPLIT_H
#define DAWN_UTIL_STRING_SPLIT_H

#include <string>
#include <vector>

#ifdef USE_ABSEIL
#include <absl/strings/str_split.h>
#endif

namespace Dawn::Utility {
#ifdef USE_ABSEIL
template <class... Args>
decltype(auto) Split(Args&&... args) {
    return absl::StrSplit(std::forward<Args>(args)...);
}
#else
std::vector<std::string> Split(const std::string& str,
                               const std::string& delim = " ");
#endif
}  // namespace Dawn::Utility

#endif