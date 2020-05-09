#pragma once

#ifndef DAWN_D_ENV_H
#define DAWN_D_ENV_H

#include <string_view>

namespace Dawn::Utility {

std::string SafeGetEnv(const char*);

}  // namespace Dawn::Utility

#endif