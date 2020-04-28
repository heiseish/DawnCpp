#include "trader_manager.hpp"
#include <algorithm>
#include <filesystem>
#include "absl/flags/flag.h"
#include "binance_trader.hpp"
#include "rapidjson/document.h"
#include "utility/general/logging.hpp"
#include "utility/general/main_flags.hpp"
#include "utility/string/from_json_file.hpp"

namespace Dawn::Trading {

namespace {

using path = std::filesystem::path;
using namespace rapidjson;

}  // namespace

BinanceTraderConfig get_binance_config([
    [maybe_unused]] const GenericValue<UTF8<char>>& doc)
{
    return BinanceTraderConfig();
}

void TraderManager::InitializeTraders()
{
    auto config_path = path(absl::GetFlag(FLAGS_config_folder));
    auto config_file = config_path / "trading.json";
    DAWN_INFO("Loading config file at {}", config_file.string());
    auto configs = Utility::FromJsonFile(config_file.string());

    Document doc;
    DAWN_STRICT_ENFORCE(!doc.Parse(configs.c_str()).HasParseError(),
                        "Failed to parse json due to {}",
                        doc.GetParseError());
    DAWN_STRICT_ENFORCE(doc.IsArray(), "Config file should be an array");

    for (SizeType i = 0; i < doc.Size(); ++i) {
        DAWN_STRICT_ENFORCE(doc[i].HasMember("id"), "Trader {} has no id", i);
        std::string id_ = doc[i]["id"].GetString();
        if (id_ == "Binance") {
            auto trader_ =
                std::make_unique<BinanceTrader>(get_binance_config(doc[i]));
            _traders.emplace_back(std::move(trader_));
        }
    }

    std::for_each(_traders.begin(), _traders.end(), [](auto& trader) {
        trader->Initialize();
    });
}

void TraderManager::ActivateAllTraders()
{
    std::for_each(_traders.begin(), _traders.end(), [](auto& trader) {
        trader->Start();
    });
}

void TraderManager::StopAllTraders()
{
    std::for_each(
        _traders.begin(), _traders.end(), [](auto& trader) { trader->Stop(); });
}

}  // namespace Dawn::Trading
