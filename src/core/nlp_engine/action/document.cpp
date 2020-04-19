#include "document.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

namespace Dawn::Core {

std::vector<Message> DocumentBaseAction::Execute([
    [maybe_unused]] MessageRequest&& request) const {
    std::vector<Message> reply;
    auto image_url = _unsplash_api.GetRandomUnsplashPhoto();
    Message initial;
    initial.set<std::string>(MessageType::Image, image_url);
    reply.emplace_back(std::move(initial));
    return reply;
}

}  // namespace Dawn::Core
