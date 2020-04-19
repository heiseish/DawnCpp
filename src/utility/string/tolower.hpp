#pragma once

#ifndef DAWN_UTIL_STRING_LOWER_H
#define DAWN_UTIL_STRING_LOWER_H

#include <string>

namespace Dawn::Utility {

void ToLower(std::string& str);
void ToLower(std::string::iterator first, std::string::iterator last);

}  // namespace Dawn::Utility

#endif