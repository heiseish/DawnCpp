#pragma once

#ifndef DAWN_UTIL_FS_PATH_JOIN_H
#define DAWN_UTIL_FS_PATH_JOIN_H

#include <filesystem>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Dawn::Utility {

namespace {

namespace fs = std::filesystem;
template <typename... Ts>
using AllStrings = typename std::enable_if<
    std::conjunction<std::is_convertible<Ts, std::string>...>::value>::type;

}  // namespace

// template <typename... Args, typename = AllStrings<Args...>>
// fs::path _join(Args&&... args) {
//     return fs::path(args) / ...;
// }

template <typename T, typename... Args, typename = AllStrings<Args...>>
std::string PathJoin(const T& initial, Args&&... args)
{
    std::vector<T> segs{std::forward<Args>(args)...};
    fs::path initial_(initial);
    for (auto& v : segs) {
        initial_ /= fs::path(std::move(v));
    }
    return initial_.string();
}

}  // namespace Dawn::Utility

#endif