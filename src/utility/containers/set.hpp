#pragma once

#ifndef DAWN_CONTAINER_SET_H
#define DAWN_CONTAINER_SET_H

#ifdef USE_ABSEIL
#include <absl/container/btree_set.h>
#else
#include <set>
#endif

namespace Dawn {

#ifdef USE_ABSEIL
template <typename V>
using set = absl::btree_set<V>;
#else
using std::set;
#endif

}  // namespace Dawn

#endif