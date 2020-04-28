#include <algorithm>
#include <string>

namespace Dawn::Utility {

void ToLower(std::string& str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}

void ToLower(const std::string::iterator first,
             const std::string::iterator last)
{
    std::transform(
        first, last, first, [](unsigned char c) { return std::tolower(c); });
}

}  // namespace Dawn::Utility
