#include <curl/curl.h>
#include <math.h>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <string>

#include "utility/general/logging.hpp"

namespace Dawn::Utility {

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
int progress_func([[maybe_unused]] void *ptr, double TotalToDownload,
                  double NowDownloaded, [[maybe_unused]] double TotalToUpload,
                  [[maybe_unused]] double NowUploaded) {
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }
    // how wide you want the progress meter to be
    int totaldotz = 40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = round(fractiondownloaded * totaldotz);
    // create the "meter"
    int ii = 0;
    std::cout << "Percentage completed: "
              << (int)round(fractiondownloaded * 100) << "% [";
    // part  that's full already
    for (; ii < dotz; ii++) std::cout << '=';
    // remaining part (spaces)
    for (; ii < totaldotz; ii++) std::cout << ' ';
    // and back to line begin - do not forget the fflush to avoid output
    // buffering problems!
    std::cout << "]\r";
    std::cout.flush();
    // if you don't return 0, the transfer will be aborted - see the
    // documentation
    return 0;
}

bool Download(const std::string &uri, const std::string &saved_path,
              bool overwrite) {
    DAWN_ENFORCE(!std::filesystem::is_directory(saved_path),
                 "saved_path need to be file path");
    DAWN_CHECK(std::filesystem::exists(saved_path) && !overwrite,
               "Cached file already exists at {}", saved_path);
    try {
        CURL *curl;
        FILE *fp;
        CURLcode res;
        curl = curl_easy_init();
        spdlog::info("Begin downloading file @{}", uri);
        if (curl) {
            fp = fopen(saved_path.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
            res = curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
    } catch (...) {
        spdlog::error("Failed to download file @{}", uri);
        return false;
    }
    return true;
}

}  // namespace Dawn::Utility
