#pragma once

#include <string_view>
#include <vector>

#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

#include "external/unsplash.hpp"
namespace Dawn::Core {

class DocumentBaseAction : public ActionExecuter {
public:
    DocumentBaseAction() = default;
    static constexpr std::string_view Name =
        "Action to respond to document sent";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;

private:
    External::UnsplashAPI _unsplash_api;
};

}  // namespace Dawn::Core
