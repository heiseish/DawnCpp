#include "twitter.hpp"

#include <string>
#include <vector>
#include "rapidjson/document.h"
#include "utility/general/logging.hpp"

namespace Dawn::External {

TwitterAPI::TwitterAPI(const std::string& access_token_key,
                       const std::string& access_token_secret,
                       const std::string& consumer_key,
                       const std::string& consumer_secret)
    : _access_token_key(access_token_key),
      _access_token_secret(access_token_secret),
      _consumer_key(consumer_key),
      _consumer_secret(consumer_secret)
{
    _api.getOAuth().setConsumerKey(_consumer_key);
    _api.getOAuth().setConsumerSecret(_consumer_secret);
    _api.getOAuth().setOAuthTokenKey(_access_token_key);
    _api.getOAuth().setOAuthTokenSecret(_access_token_secret);
}

/**
 * @brief Get the recent timeline posts on user profile
 * Prune the list to `limit` size if `limit` is provided.
 * @arg limit number of articles to be returned
 * @return list of tweets of `min(limit, all_tweets.size())` size
 */
std::vector<Tweet> TwitterAPI::RecentTimelinePosts(
    const std::string& profile_name,
    const unsigned long& limit) const
{
    std::string result;
    std::vector<Tweet> return_;
    if (_api.timelineUserGet(true, true, limit, profile_name)) {
        _api.getLastWebResponse(result);
        rapidjson::Document tweets_raw;
        if (tweets_raw.Parse(result.c_str()).HasParseError()) {
            spdlog::error("Failed to parse {} as json", result);
            return return_;
        }
        for (rapidjson::SizeType i = 0; i < tweets_raw.Size(); ++i) {
            Tweet tweet_;
            tweet_.text = tweets_raw[i]["text"].GetString();
            tweet_.favorite_count = tweets_raw[i]["favorite_count"].GetInt64();
            tweet_.retweet_count = tweets_raw[i]["retweet_count"].GetInt64();
            if (tweets_raw[i].HasMember("extended_entities") &&
                tweets_raw[i]["extended_entities"].HasMember("media")) {
                for (rapidjson::SizeType j = 0;
                     j < tweets_raw[i]["extended_entities"]["media"].Size();
                     ++j) {
                    tweet_.media.emplace_back(TweetMedia{
                        tweets_raw[i]["extended_entities"]["media"][j]["type"]
                            .GetString(),
                        tweets_raw[i]["extended_entities"]["media"][j]
                                  ["media_url_https"]
                                      .GetString()});
                }
            }
            return_.emplace_back(std::move(tweet_));
        }
        return return_;
    }
    _api.getLastCurlError(result);
    spdlog::error("Error while calling twitter API {}", result);
    return return_;
}

}  // namespace Dawn::External
