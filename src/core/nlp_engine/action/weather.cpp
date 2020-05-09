#include "weather.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "external/forecast.hpp"
#include "fmt/format.h"
#include "utility/general/env.hpp"

namespace Dawn::Core {

WeatherBaseAction::WeatherBaseAction()
    : _forecast_io(External::ForecastAPI(Utility::SafeGetEnv("DARKSKY_KEY")))
{
}

std::vector<Message> WeatherBaseAction::Execute([
    [maybe_unused]] MessageRequest&& request) const
{
    std::vector<Message> reply;
    External::WeatherDetails weather_details;
    if (request.message.isLocation()) {
        weather_details = _forecast_io.CurrentWeather(
            request.message.get<Geography::GeographicCoordinates>());
    }
    else
        weather_details = _forecast_io.CurrentWeather();
    Message current_weather, summary_weather;
    auto current_weather_str = fmt::format(
        "Hey {}, {}", request.user_info.first_name, weather_details.current);
    current_weather.set<std::string>(MessageType::Text, current_weather_str);
    summary_weather.set<std::string>(MessageType::Text,
                                     weather_details.summary);

    reply.emplace_back(std::move(current_weather));
    reply.emplace_back(std::move(summary_weather));
    return reply;
}

}  // namespace Dawn::Core
