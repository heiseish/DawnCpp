#pragma once

#ifndef DAWN_UTILITY_URL_H
#define DAWN_UTILITY_URL_H

#include <curl/curl.h>
#include <string>
namespace Dawn::Utility {

class URLParser {
public:
    static void EncodeURL(std::string& uri);
    static std::string GetFinalRedirectedUrl(const std::string& uri,
                                             CURL* curl = nullptr);
};

}  // namespace Dawn::Utility

#endif