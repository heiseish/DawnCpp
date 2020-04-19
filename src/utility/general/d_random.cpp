#include "d_random.hpp"
#include <random>

namespace Dawn::Utility {

std::mt19937 &GetRandomGenerator() {
    // initialize once per thread
    thread_local static std::random_device srd;
    thread_local static std::mt19937 smt(srd());
    return smt;
}

}  // namespace Dawn::Utility