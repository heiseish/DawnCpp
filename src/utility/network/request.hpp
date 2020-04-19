#pragma once

#ifndef DAWN_REQUEST_H
#define DAWN_REQUEST_H

#include <curl/curl.h>
#include <optional>
#include <vector>
#include "rapidjson/document.h"

namespace Dawn::Utility {

enum RequestMode { GET, POST };

class Requester {
public:
    Requester(const bool ipv6 = false, const int timeout = 10);
    ~Requester();
    std::optional<rapidjson::Document> GetRequest(const std::string& uri) const;
    std::optional<rapidjson::Document> PostRequest(
        const std::string& uri) const;

private:
    std::optional<rapidjson::Document> request(const std::string& url,
                                               RequestMode mode) const;
    mutable CURL* _curl = nullptr;
};

}  // namespace Dawn::Utility

#endif