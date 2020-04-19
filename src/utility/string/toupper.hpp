#pragma once

#ifndef DAWN_UTIL_STRING_UPPER_H
#define DAWN_UTIL_STRING_UPPER_H

#include <string>

namespace Dawn::Utility {

void ToUpper(std::string& str);
void ToUpper(std::string::iterator first, std::string::iterator last);

}  // namespace Dawn::Utility

#endif