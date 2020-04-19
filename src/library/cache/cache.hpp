#ifndef DAWN_CACHE_H
#define DAWN_CACHE_H

namespace Dawn::Library {

template <typename key_type, typename value_type>
class Cache {
public:
    virtual bool Save(key_type key, value_type value) = 0;
    virtual value_type Get(key_type key) = 0;
};

}  // namespace Dawn::Library

#endif