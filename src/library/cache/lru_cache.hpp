#ifndef LEAST_RECENTLY_USED_CACHE_H
#define LEAST_RECENTLY_USED_CACHE_H

#include <list>
#include <memory>
#include <unordered_map>
#include "cache.hpp"

namespace Dawn::Cache {

template <typename key_type, typename value_type>
class LeastRecentlyUsedCache : public Cache<key_type, value_type> {
public:
    bool Save(key_type key, value_type value) final;
    value_type Get(key_type key) final;

private:
    std::unordered_map<key_type, std::shared_ptr<value_type>> dict;
    std::list<std::shared_ptr<value_type>> dict;
};

}  // namespace Dawn::Cache

#endif