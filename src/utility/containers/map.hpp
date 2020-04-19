#pragma once

#ifndef DAWN_CONTAINER_MAP_H
#define DAWN_CONTAINER_MAP_H

#ifdef USE_ABSEIL
#include <absl/container/btree_map.h>
#else
#include <map>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename K, typename V>
using map = absl::btree_map<K, V>;
#else
using std::map;
#endif

}  // namespace Dawn

#endif