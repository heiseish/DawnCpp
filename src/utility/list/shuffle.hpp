#pragma once

#ifndef DAWN_UTIL_SHUFFLE_H
#define DAWN_UTIL_SHUFFLE_H
#include <algorithm>
#include <vector>
#include "utility/general/d_random.hpp"

namespace Dawn::Utility {

template <typename T>
void Shuffle(std::vector<T>& in_list)
{
    std::shuffle(in_list.begin(), in_list.end(), GetRandomGenerator());
}

template <typename T>
void Shuffle(typename std::vector<T>::iterator first,
             typename std::vector<T>::iterator last)
{
    std::shuffle(first, last, GetRandomGenerator());
}

}  // namespace Dawn::Utility

#endif