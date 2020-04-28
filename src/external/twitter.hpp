#pragma once

#ifndef EXTERNAL_TWITTER_API_H
#define EXTERNAL_TWITTER_API_H

#include <string>
#include <vector>
#include "twitcurl/twitcurl.h"

namespace Dawn::External {

class TweetMedia {
public:
    std::string type;
    std::string media_url_https;
};

class Tweet {
public:
    std::string created_at;
    std::string text;
    int64_t favorite_count;
    int64_t retweet_count;
    std::vector<TweetMedia> media;
};

class TwitterAPI {
public:
    constexpr static const std::string_view WebsocketHost =
        "https://stream.twitter.com/1.1";

    TwitterAPI(const std::string& access_token_key,
               const std::string& access_token_secret,
               const std::string& consumer_key,
               const std::string& consumer_secret);

    /**
     * @brief Get the recent timeline posts on user profile
     * Prune the list to `limit` size if `limit` is provided.
     * @arg limit number of articles to be returned
     * @return list of tweets of `min(limit, all_tweets.size())` size
     */
    std::vector<Tweet> RecentTimelinePosts(
        const std::string& profile_name,
        const unsigned long& limit = 5) const;

private:
    mutable twitCurl _api;
    const std::string _access_token_key;
    const std::string _access_token_secret;
    const std::string _consumer_key;
    const std::string _consumer_secret;
};

}  // namespace Dawn::External

#endif