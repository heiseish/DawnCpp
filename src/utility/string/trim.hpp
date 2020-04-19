#pragma once

#ifndef DAWN_UTIL_STRING_TRIM_H
#define DAWN_UTIL_STRING_TRIM_H

#include <string>

namespace Dawn::Utility {

void Trim(std::string& str);
void TrimLeading(std::string& str);
void TrimTrailing(std::string& str);
void TrimExtraSpaces(std::string& str);

}  // namespace Dawn::Utility

#endif