#pragma once

#ifndef DAWN_UTILTIY_ENCRYPT_H
#define DAWN_UTILTIY_ENCRYPT_H

#include <string>

namespace Dawn::Utility {

std::string hmac_sha256(const char *key, const char *data);
std::string sha256(const char *data);

}  // namespace Dawn::Utility

#endif