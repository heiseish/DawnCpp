#pragma once

#include <string>
#include "utility/geography/coordinates.hpp"
#include "utility/network/request.hpp"

namespace Dawn::External {

class WeatherDetails {
public:
    WeatherDetails() {}

    WeatherDetails(const std::string& _current, const std::string& _summary)
        : current(_current), summary(_summary)
    {
    }
    std::string current;
    std::string summary;
};

class ForecastAPI {
public:
    ForecastAPI(const std::string& api_key);
    ~ForecastAPI();
    /**
     * @brief Get weather description for weather at `location`
     * Call dark sky api to obtain the necessary information
     * @arg location Location for weather to be check
     * @return a description of current weather in the area of `location`
     */
    WeatherDetails CurrentWeather(
        const Geography::GeographicCoordinates& location = {1.28891123,
                                                            103.7768478}) const;

private:
    std::string _api_key;
    Utility::Requester _request;
};

}  // namespace Dawn::External