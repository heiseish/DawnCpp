#include "response.hpp"

#include <functional>
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
    _executers.reserve(5);

    _executers.emplace_back(std::make_unique<DocumentBaseAction>());
    _mapper.emplace(BaseActionType::Document,
                    std::bind(&ActionExecuter::Execute,
                              _executers.back().get(),
                              std::placeholders::_1));
    _executers.emplace_back(std::make_unique<NewsBaseAction>());
    _mapper.emplace(BaseActionType::News,
                    std::bind(&ActionExecuter::Execute,
                              _executers.back().get(),
                              std::placeholders::_1));
    _executers.emplace_back(std::make_unique<PokemonGoBaseAction>());
    _mapper.emplace(BaseActionType::PokemonGo,
                    std::bind(&ActionExecuter::Execute,
                              _executers.back().get(),
                              std::placeholders::_1));
    _executers.emplace_back(std::make_unique<UnknownBaseAction>());
    _mapper.emplace(BaseActionType::Unknown,
                    std::bind(&ActionExecuter::Execute,
                              _executers.back().get(),
                              std::placeholders::_1));
    _executers.emplace_back(std::make_unique<WeatherBaseAction>());
    _mapper.emplace(BaseActionType::Weather,
                    std::bind(&ActionExecuter::Execute,
                              _executers.back().get(),
                              std::placeholders::_1));
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
    res.message = _mapper[intent](std::move(request));
    return res;
}

}  // namespace Dawn::Core
