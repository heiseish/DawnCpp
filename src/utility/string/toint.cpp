#include "toint.hpp"
#include <string>

namespace Dawn::Utility {

int64_t ToInt64(const std::string& str) { return std::stoll(str); }
int ToInt(const std::string& str) { return std::stoi(str); }

}  // namespace Dawn::Utility
