#include "response.hpp"

#include <memory>
#include <string>

#include "core/nlp_engine/action/action_types.hpp"
#include "core/nlp_engine/action/document.hpp"
#include "core/nlp_engine/action/news.hpp"
#include "core/nlp_engine/action/pokemon_go.hpp"
#include "core/nlp_engine/action/unknown.hpp"
#include "core/nlp_engine/action/weather.hpp"

#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"

namespace Dawn::Core {

TextResponder::TextResponder()
{
    _executers.emplace(BaseActionType::Document,
                       std::make_unique<DocumentBaseAction>());
    _executers.emplace(BaseActionType::News,
                       std::make_unique<NewsBaseAction>());
    _executers.emplace(BaseActionType::PokemonGo,
                       std::make_unique<PokemonGoBaseAction>());
    _executers.emplace(BaseActionType::Unknown,
                       std::make_unique<UnknownBaseAction>());
    _executers.emplace(BaseActionType::Weather,
                       std::make_unique<WeatherBaseAction>());
}

MessageResponse TextResponder::GenerateResponse(MessageRequest&& request)
{
    MessageResponse res;
    res.user_info = request.user_info;
    res.platform_type = request.platform_type;
    BaseActionType intent = BaseActionType::Document;
    if (request.message.is(MessageType::Text)) {
        auto text_msg = request.message.get<std::string>();
        intent = _text_classifier.classify(text_msg);
    }
    else if (request.message.is(MessageType::Location)) {
        intent = BaseActionType::Weather;
    }
    res.message = _executers[intent]->Execute(std::move(request));
    return res;
}

}  // namespace Dawn::Core
