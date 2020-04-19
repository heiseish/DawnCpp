#include "toupper.hpp"
#include <algorithm>
#include <string>

namespace Dawn::Utility {

void ToUpper(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
}

void ToUpper(const std::string::iterator first,
             const std::string::iterator last) {
    std::transform(first, last, first,
                   [](unsigned char c) { return std::toupper(c); });
}

}  // namespace Dawn::Utility
