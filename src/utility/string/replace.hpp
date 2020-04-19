#pragma once

#ifndef DAWN_UTIL_STRING_REPLACE_H
#define DAWN_UTIL_STRING_REPLACE_H

#include <string>

namespace Dawn::Utility {

std::string Replace(const std::string& str,
                    const std::string& substr_to_replace,
                    const std::string& replacement = "",
                    unsigned int offset = 0u,
                    const bool& case_sensitive = true);
std::string ReplaceAll(const std::string& str,
                       const std::string& substr_to_replace,
                       const std::string& replacement = "",
                       const bool& case_sensitive = true);

}  // namespace Dawn::Utility

#endif