#pragma once

#ifndef EXTERNAL_UNSPLASH_H
#define EXTERNAL_UNSPLASH_H

#include <curl/curl.h>
#include <string>
#include <string_view>
#include <vector>
#include "utility/general/d_random.hpp"

namespace Dawn::External {

class UnsplashAPI {
public:
    UnsplashAPI();
    ~UnsplashAPI();
    std::string GetRandomUnsplashPhoto() const;

private:
    CURL* _curl = nullptr;
    const std::vector<std::string_view> _topics{
        "beach", "funny", "city", "sea", "rainforest", "starrynight"};
};

}  // namespace Dawn::External

#endif