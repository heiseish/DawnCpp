#include "unknown.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

#include "fmt/format.h"

namespace Dawn::Core {

std::vector<Message> UnknownBaseAction::Execute([
    [maybe_unused]] MessageRequest&& request) const {
    std::vector<Message> reply;
    Message initial;
    initial.set<std::string>(MessageType::Text, "...");
    reply.emplace_back(std::move(initial));
    return reply;
}

}  // namespace Dawn::Core
