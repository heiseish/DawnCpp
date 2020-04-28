#include "url.hpp"
#include <curl/curl.h>
#include <string>
#include "utility/string/replace.hpp"

namespace Dawn::Utility {

void URLParser::EncodeURL(std::string& uri)
{
    uri = ReplaceAll(uri, " ", "%20");
}

size_t write_data([[maybe_unused]] void* buffer,
                  size_t size,
                  size_t nmemb,
                  [[maybe_unused]] void* userp)
{
    return size * nmemb;
}

std::string URLParser::GetFinalRedirectedUrl(const std::string& uri, CURL* curl)
{
    bool owned_curl = false;
    if (curl == nullptr) {
        curl = curl_easy_init();
        owned_curl = true;
    }
    // Grab URL and pass it to the variable.
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    // // Catch output (do NOT print!)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    // Return follow location true
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return "";
    }
    // Getinfo or redirected URL from effective URL
    char* final_url = NULL;
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &final_url);
    // Close handle
    if (owned_curl)
        curl_easy_cleanup(curl);
    return std::string(final_url);
}

}  // namespace Dawn::Utility
