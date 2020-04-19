#pragma once

#ifndef CORE_NLP_BASE_WEATHER_H
#define CORE_NLP_BASE_WEATHER_H

#include <string_view>
#include <vector>

#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "external/forecast.hpp"
namespace Dawn::Core {

class WeatherBaseAction : public ActionExecuter {
public:
    WeatherBaseAction();
    static constexpr std::string_view Name =
        "Action to get latest weather news from DarkSky";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;

private:
    External::ForecastAPI _forecast_io;
};

}  // namespace Dawn::Core
#endif
