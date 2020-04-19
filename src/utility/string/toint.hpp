#pragma once

#ifndef DAWN_UTIL_STRING_TOINT_H
#define DAWN_UTIL_STRING_TOINT_H

#include <string>

namespace Dawn::Utility {

int64_t ToInt64(const std::string& str);
int ToInt(const std::string& str);

}  // namespace Dawn::Utility

#endif