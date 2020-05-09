#include "binance_trader.hpp"
#include <rapidjson/writer.h>
#include <cstdio>
#include "external/binance.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "utility/filesystem/join.hpp"
#include "utility/general/env.hpp"
#include "utility/general/logging.hpp"
#include "utility/general/main_flags.hpp"
#include "utility/network/websocket.hpp"

namespace Dawn::Trading {

namespace {
using namespace rapidjson;

}

BinanceTrader::BinanceTrader(const BinanceTraderConfig& config) noexcept
    : _config(config),
      _binance_api(Utility::SafeGetEnv("BINANCE_KEY"),
                   Utility::SafeGetEnv("BINANCE_SECRET"))
{
}

BinanceTrader::BinanceTrader(BinanceTraderConfig&& config) noexcept
    : _config(std::move(config)),
      _binance_api(Utility::SafeGetEnv("BINANCE_KEY"),
                   Utility::SafeGetEnv("BINANCE_SECRET"))
{
}

BinanceTrader::~BinanceTrader() { Stop(); }

void BinanceTrader::Initialize() { DAWN_INFO("BinanceTrader initializing"); }

void BinanceTrader::DownloadHistoricalData()
{
    auto res = _binance_api.GetExchangeInfo();
    Document doc;
    DAWN_STRICT_ENFORCE(!doc.Parse(res.c_str()).HasParseError(),
                        "Failed to parse json due to {}",
                        doc.GetParseError());
    std::string data_folder_path = absl::GetFlag(FLAGS_data_folder);
    auto exchange_file = Utility::PathJoin(data_folder_path, "exchange.json");
    DAWN_INFO(exchange_file);
    FILE* fp = fopen(exchange_file.c_str(), "wb");  // non-Windows use "w"
    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
    Writer<FileWriteStream> writer(os);
    doc.Accept(writer);
    fclose(fp);
}

void BinanceTrader::Start()
{
    _listen_key = _binance_api.StartUserDataStream();
}

void BinanceTrader::Stop()
{
    _binance_api.CloseUserDataStream(_listen_key);
    Utility::Websocket::StopEventLoop();
}

}  // namespace Dawn::Trading
