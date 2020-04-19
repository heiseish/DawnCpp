#pragma once

#ifndef DAWN_CONTAINER_UNORDERED_SET_H
#define DAWN_CONTAINER_UNORDERED_SET_H

#ifdef USE_ABSEIL
#include <absl/container/flat_hash_set.h>
#else
#include <unordered_set>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename V>
using unordered_set = absl::flat_hash_set<V>;
#else
using std::unordered_set;
#endif

}  // namespace Dawn

#endif