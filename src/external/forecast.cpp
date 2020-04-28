#include "forecast.hpp"

#include <optional>
#include <string>

#include <curl/curl.h>
#include "delegate/Delegate.h"
#include "fmt/format.h"
#include "utility/general/retry.hpp"
#include "utility/network/request.hpp"
#include "utility/string/tolower.hpp"

namespace Dawn::External {

ForecastAPI::ForecastAPI(const std::string& api_key) : _api_key(api_key) {}
ForecastAPI::~ForecastAPI() {}

WeatherDetails ForecastAPI::CurrentWeather(
    const Geography::GeographicCoordinates& location) const
{
    std::string uri = fmt::format("https://api.darksky.net/forecast/{}/{},{}",
                                  _api_key,
                                  location.latitude,
                                  location.longitude);
    // request function delegate
    SA::delegate<std::optional<rapidjson::Document>()> func_delegate;
    decltype(auto) request_call =
        [&uri, this]() -> std::optional<rapidjson::Document> {
        return _request.GetRequest(uri);
    };
    decltype(func_delegate) drequest_call = request_call;

    // Callback function delegate
    SA::delegate<bool(const std::optional<rapidjson::Document>&)>
        callback_delegate;
    decltype(auto) callback_call =
        [](const std::optional<rapidjson::Document>& doc) -> bool {
        return doc && doc->IsObject() && doc->HasMember("currently") &&
               doc->HasMember("hourly") && doc->HasMember("daily") &&
               (*doc)["currently"].HasMember("summary") &&
               (*doc)["hourly"].HasMember("summary") &&
               (*doc)["daily"].HasMember("summary");
    };
    decltype(callback_delegate) dcallback_call = callback_call;
    std::optional<rapidjson::Document> doc;
    try {
        doc = Utility::Retry(drequest_call, dcallback_call);
    }
    catch (const std::runtime_error& e) {
        return {};
    }
    if (!doc)
        return {};
    std::string currently_summary = (*doc)["currently"]["summary"].GetString();
    Utility::ToLower(currently_summary);
    std::string hourly_summary = (*doc)["hourly"]["summary"].GetString();
    Utility::ToLower(hourly_summary);
    std::string daily_summary = (*doc)["daily"]["summary"].GetString();
    Utility::ToLower(daily_summary);
    WeatherDetails result;
    result.current = fmt::format("it's {} near your house now!",
                                 std::move(currently_summary));
    result.summary =
        fmt::format("In general, it is {} And this week there is {}",
                    std::move(hourly_summary),
                    std::move(daily_summary));
    return result;
}

}  // namespace Dawn::External
