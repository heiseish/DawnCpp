#include "newsapi.hpp"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include "delegate/Delegate.h"
#include "fmt/format.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "utility/general/logging.hpp"
#include "utility/general/retry.hpp"
#include "utility/network/request.hpp"
#include "utility/network/url.hpp"
#include "utility/string/replace.hpp"

namespace Dawn::External {

std::ostream& operator<<(std::ostream& os, NewsAPIArticle const& m) {
    return os << "{ title:" << m.title << ", author: " << m.author
              << ", url: " << m.url << ", urlToImage: " << m.urlToImage
              << " } ";
}

NewsAPI::NewsAPI(const std::string& api_key, const std::string& sources)
    : _api_key(api_key), _sources(sources) {}

NewsAPI::~NewsAPI() {}

/**
 * @brief check if an article entry is valid that can be used as api response.
 * @arg article article value obtained from json response to be checked
 * @return whether an article entry is not valid
 */
bool not_valid_article(const rapidjson::Value& article) {
    return !article.HasMember("title") || !article["title"].IsString() ||
           !article.HasMember("author") || !article["author"].IsString() ||
           !article.HasMember("url") || !article["url"].IsString() ||
           !article.HasMember("urlToImage") ||
           !article["urlToImage"].IsString();
}

std::vector<NewsAPIArticle> NewsAPI::TopHeadlines(const std::string& query,
                                                  const unsigned long& limit,
                                                  const bool& shuffle) const {
    std::string uri =
        fmt::format("https://newsapi.org/v2/top-headlines?sources={}&apiKey={}",
                    _sources, _api_key);
    if (!query.empty()) {  // attach query to the uri
        auto trimmed_query = query;
        Utility::URLParser::EncodeURL(trimmed_query);
        uri = fmt::format("{}&q={}", uri, trimmed_query);
    }
    // request function delegate
    SA::delegate<std::optional<rapidjson::Document>()> func_delegate;
    decltype(auto) request_call =
        [&uri, this]() -> std::optional<rapidjson::Document> {
        return _request.GetRequest(uri);
    };
    decltype(func_delegate) drequest_call = request_call;

    // Callback function delegate
    SA::delegate<bool(const std::optional<rapidjson::Document>&)>
        callback_delegate;
    decltype(auto) callback_call =
        [](const std::optional<rapidjson::Document>& doc) -> bool {
        return doc && doc->IsObject() && doc->HasMember("articles");
    };
    decltype(callback_delegate) dcallback_call = callback_call;

    std::optional<rapidjson::Document> res;
    try {
        res = Utility::Retry(drequest_call, dcallback_call);
    } catch (const std::runtime_error& e) {
        return {};
    }
    if (!res) return {};
    std::vector<NewsAPIArticle> articles;
    const rapidjson::Value& articles_r = (*res)["articles"];
    if (!articles_r.IsArray()) return {};
    for (rapidjson::SizeType i = 0; i < articles_r.Size();
         ++i) {  // rapidjson uses SizeType instead of size_t.
        if (not_valid_article(articles_r[i])) continue;
        articles.emplace_back(articles_r[i]["title"].GetString(),
                              articles_r[i]["author"].GetString(),
                              articles_r[i]["url"].GetString(),
                              articles_r[i]["urlToImage"].GetString());
    }
    if (shuffle) {
        Utility::Shuffle(articles);
    }
    size_t can_have = std::min(limit, articles.size());
    std::vector<NewsAPIArticle> to_return;
    for (unsigned int i = 0; i < can_have; ++i) {
        to_return.emplace_back(std::move(articles.at(i)));
    }
    return to_return;
}

}  // namespace Dawn::External
