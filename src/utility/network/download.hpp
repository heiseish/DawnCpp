#pragma once

#ifndef DAWN_DOWNLOAD_H
#define DAWN_DOWNLOAD_H
#include <string>
namespace Dawn::Utility {

bool Download(const std::string& uri, const std::string& saved_path,
              bool overwrite = false) noexcept;

}

#endif