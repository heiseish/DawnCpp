#pragma once

#include <string_view>
#include <vector>

#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "external/twitter.hpp"

namespace Dawn::Core {

class PokemonGoBaseAction : public ActionExecuter {
public:
    PokemonGoBaseAction();
    static constexpr std::string_view Name =
        "Action to get latest pokemon go news from twitter";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;

private:
    External::TwitterAPI _api;
};

}  // namespace Dawn::Core
