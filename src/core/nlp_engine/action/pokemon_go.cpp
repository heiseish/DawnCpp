#include "pokemon_go.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "external/twitter.hpp"
#include "fmt/format.h"
#include "utility/general/env.hpp"
#include "utility/general/logging.hpp"

namespace Dawn::Core {

PokemonGoBaseAction::PokemonGoBaseAction()
    : _api(External::TwitterAPI(
          Utility::SafeGetEnv("TWITTER_ACCESS_TOKEN_KEY"),
          Utility::SafeGetEnv("TWITTER_ACCESS_TOKEN_SECRET"),
          Utility::SafeGetEnv("TWITTER_CONSUMER_KEY"),
          Utility::SafeGetEnv("TWITTER_CONSUMER_SECRET")))
{
}

std::vector<Message> PokemonGoBaseAction::Execute(
    [[maybe_unused]] MessageRequest&& request) const
{
    std::vector<Message> reply;
    auto tweets = _api.RecentTimelinePosts("PokemonGoApp");
    for (const auto& tweet : tweets) {
        Message msg_;
        msg_.set<std::string>(MessageType::Text, tweet.text);
        reply.emplace_back(std::move(msg_));
        for (const auto& media_att : tweet.media) {
            Message msg_;
            MessageType media_type = MessageType::Text;
            if (media_att.type == "photo")
                media_type = MessageType::Image;
            if (media_att.type == "video")
                media_type = MessageType::Video;
            msg_.set<std::string>(media_type, media_att.media_url_https);
            reply.emplace_back(std::move(msg_));
        }
    }
    return reply;
}

}  // namespace Dawn::Core
