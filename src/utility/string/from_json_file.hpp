#pragma once

#ifndef DAWN_UTIL_STRING_FROM_JSON_FILE_H
#define DAWN_UTIL_STRING_FROM_JSON_FILE_H

#include <string>

namespace Dawn::Utility {

std::string FromJsonFile(const std::string& file_path);

}  // namespace Dawn::Utility

#endif