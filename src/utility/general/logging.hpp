#pragma once

#ifndef DAWNLOGGER_H
#define DAWNLOGGER_H

#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "fmt/format.h"
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#include "spdlog/spdlog.h"
namespace Dawn {

#define DAWN_WARN SPDLOG_WARN
#define DAWN_INFO SPDLOG_INFO
#define DAWN_DEBUG SPDLOG_DEBUG
#define DAWN_ERROR SPDLOG_ERROR

void InitLogger();

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (auto& e : v)
        os << e << ',';
    os << "]\n";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unordered_set<T>& v)
{
    os << "[";
    for (auto& e : v)
        os << e << ',';
    os << "]\n";
    return os;
}
template <typename T, typename K>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, K>& v)
{
    os << "[";
    for (auto& e : v)
        os << e.first << ':' << e.second << ',';
    os << "]\n";
    return os;
}

#define DAWN_ENFORCE(x, ...)         \
    do {                             \
        if (!(x)) {                  \
            DAWN_ERROR(__VA_ARGS__); \
            return {};               \
        }                            \
    } while (0)

#define DAWN_STRICT_ENFORCE(x, ...)                             \
    do                                                          \
        if (!(x))                                               \
            throw std::runtime_error(fmt::format(__VA_ARGS__)); \
    while (0)

#define DAWN_CHECK(x, ...)           \
    do {                             \
        if (x) {                     \
            DAWN_ERROR(__VA_ARGS__); \
            return true;             \
        }                            \
    } while (0)

#endif
}