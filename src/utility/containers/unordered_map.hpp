#pragma once

#ifndef DAWN_CONTAINER_UNORDERED_MAP_H
#define DAWN_CONTAINER_UNORDERED_MAP_H

#ifdef USE_ABSEIL
#include <absl/container/flat_hash_map.h>
#else
#include <unordered_map>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename K, typename V>
using unordered_map = absl::flat_hash_map<K, V>;
#else
using std::unordered_map;
#endif

}  // namespace Dawn

#endif