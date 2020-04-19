#pragma once

#ifndef DAWN_CONTAINER_MULTIMAP_H
#define DAWN_CONTAINER_MULTIMAP_H

#ifdef USE_ABSEIL
#include <absl/container/btree_multimap.h>
#else
#include <multimap>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename K, typename V>
using multimap = absl::btree_multimap<K, V>;
#else
using std::multimap;
#endif

}  // namespace Dawn

#endif