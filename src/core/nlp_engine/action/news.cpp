#include "news.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "external/newsapi.hpp"
#include "fmt/format.h"
#include "utility/general/env.hpp"
#include "utility/string/replace.hpp"
#include "utility/string/tolower.hpp"
#include "utility/string/trim.hpp"

namespace Dawn::Core {

NewsBaseAction::NewsBaseAction()
    : _news_api(External::NewsAPI(Utility::SafeGetEnv("NEWSAPI_KEY")))
{
}

std::vector<Message> NewsBaseAction::Execute(MessageRequest&& request) const
{
    std::vector<Message> reply;
    auto text_msg = request.message.get<std::string>();
    // removed any news keyword
    Utility::ToLower(text_msg);
    auto trimmed_query = Utility::ReplaceAll(text_msg, "news");
    Utility::Trim(trimmed_query);

    auto articles = _news_api.TopHeadlines(trimmed_query);
    Message initial;
    if (articles.empty()) {
        initial.set<std::string>(MessageType::Text,
                                 "Sorry I cannot get any news right now!");
    }
    else {
        initial.set<std::string>(MessageType::Text,
                                 fmt::format("Hey {}, here are the top news:",
                                             request.user_info.first_name));
    }
    reply.emplace_back(std::move(initial));
    for (const auto& article : articles) {
        Message article_details;
        article_details.set<std::string>(
            MessageType::Text,
            fmt::format("{}. Link: {}", article.title, article.url));
        reply.emplace_back(std::move(article_details));
        // if (request.platform_type == PlatformType::Telegram)
        //     continue;  // telegram has preview which show the image by
        //     default
        Message article_image;
        article_image.set<std::string>(MessageType::Image, article.urlToImage);
        reply.emplace_back(std::move(article_image));
    }
    return reply;
}

}  // namespace Dawn::Core
