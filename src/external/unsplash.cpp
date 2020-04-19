#include "unsplash.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "fmt/format.h"
#include "utility/general/d_random.hpp"
#include "utility/network/url.hpp"

namespace Dawn::External {

UnsplashAPI::UnsplashAPI() : _curl(curl_easy_init()) {}
UnsplashAPI::~UnsplashAPI() {
    if (_curl != nullptr) {
        curl_easy_cleanup(_curl);
    }
}

std::string UnsplashAPI::GetRandomUnsplashPhoto() const {
    std::vector<std::string_view> out;
    std::sample(_topics.begin(), _topics.end(), std::back_inserter(out), 1,
                Utility::GetRandomGenerator());
    const std::string uri =
        fmt::format("https://source.unsplash.com/1600x900/?{}", out.back());
    decltype(auto) final_uri =
        Utility::URLParser::GetFinalRedirectedUrl(uri, _curl);
    return final_uri;
}

}  // namespace Dawn::External