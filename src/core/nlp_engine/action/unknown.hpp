#pragma once

#include <string_view>
#include <vector>

#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

namespace Dawn::Core {

class UnknownBaseAction : public ActionExecuter {
public:
    UnknownBaseAction() = default;
    static constexpr std::string_view Name =
        "Action to respond to unknown intent";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;
};

}  // namespace Dawn::Core
