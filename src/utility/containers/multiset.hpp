#pragma once

#ifndef DAWN_CONTAINER_MULTISET_H
#define DAWN_CONTAINER_MULTISET_H

#ifdef USE_ABSEIL
#include <absl/container/btree_multiset.h>
#else
#include <multiset>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename V>
using multiset = absl::btree_multiset<V>;
#else
using std::multiset;
#endif

}  // namespace Dawn

#endif