#include "request.hpp"

#include <curl/curl.h>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "utility/general/logging.hpp"
namespace Dawn::Utility {

std::size_t callback(const char* in,
                     std::size_t size,
                     std::size_t num,
                     std::string* out)
{
    const std::size_t totalBytes(size * num);
    out->append(in, totalBytes);
    return totalBytes;
}

Requester::Requester(const bool ipv6, const int timeout)
{
    _curl = curl_easy_init();
    // Don't bother trying IPv6, which would increase DNS resolution time.
    if (ipv6)
        curl_easy_setopt(_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);
    else
        curl_easy_setopt(_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    // Don't wait forever, time out after 10 seconds.
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, timeout);
    // Follow HTTP redirects if necessary.
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    // Response information.
    // Hook up data handling function.
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, callback);
}

Requester::~Requester()
{
    if (_curl != nullptr) {
        curl_easy_cleanup(_curl);
    }
}

std::optional<rapidjson::Document> Requester::request(
    const std::string& url,
    RequestMode mode = RequestMode::GET) const
{
    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    if (mode == RequestMode::POST) {
        curl_easy_setopt(_curl, CURLOPT_POST, 1);
    }
    // Hook up data container (will be passed as the last parameter to the
    // callback handling function).  Can be any pointer type, since it will
    // internally be passed as a void pointer.
    long httpCode_{0};
    std::unique_ptr<std::string> httpData_ = std::make_unique<std::string>();
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, httpData_.get());
    curl_easy_perform(_curl);
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, &httpCode_);
    if (httpCode_ != 200) {
        DAWN_ERROR("Failed to get request response from {}", url);
        return {};
    }
    // Response looks good - done using Curl now.  Try to parse the results
    // and print them out.
    rapidjson::Document d;
    if (d.Parse(httpData_.get()->c_str()).HasParseError()) {
        DAWN_ERROR("Failed to parse json response from {}", url);
        return {};
    }
    return d;
}

std::optional<rapidjson::Document> Requester::GetRequest(
    const std::string& url) const
{
    return request(url);
}

std::optional<rapidjson::Document> Requester::PostRequest(
    const std::string& url) const
{
    return request(url, RequestMode::POST);
}

}  // namespace Dawn::Utility