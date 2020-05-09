#pragma once

#include <cstdlib>
#include <string_view>
#include <vector>

#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

#include "external/newsapi.hpp"

namespace Dawn::Core {

class NewsBaseAction : public ActionExecuter {
public:
    NewsBaseAction();
    static constexpr std::string_view Name = "Action to get news";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;

private:
    External::NewsAPI _news_api;
};

}  // namespace Dawn::Core
