#include "utility/network/websocket.hpp"
#include <chrono>
#include <cstdlib>
#include <memory>
#include <string>
#include <thread>
#include "delegate/Delegate.h"
#include "external/binance.hpp"
#include "fmt/format.h"
#include "gtest/gtest.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "utility/containers/map.hpp"
#include "utility/general/env.hpp"
#include "utility/general/logging.hpp"

namespace Dawn::Utility {

class WebsocketTest : public ::testing::Test {
protected:
    map<long, map<std::string, double> > _klinesCache;
    std::unique_ptr<External::BinanceAPI> binance_api_;
    virtual void SetUp()
    {
        GTEST_SKIP();
        binance_api_ = std::make_unique<External::BinanceAPI>(
            Utility::SafeGetEnv("BINANCE_KEY"),
            Utility::SafeGetEnv("BINANCE_SECRET"));
        Websocket::Initialize();
    }

    virtual void TearDown() { Websocket::StopEventLoop(); }

    //---------------
public:
    void print_klinesCache()
    {
        auto result_ = fmt::format("\n==========================\n");
        for (auto& v : _klinesCache) {
            auto start_of_candle = v.first;
            auto candle_obj = v.second;
            result_ =
                fmt::format("{}s: {}, o: {}, h: {}, l: {}, c: {}, v: {}\n",
                            result_,
                            start_of_candle,
                            candle_obj["o"],
                            candle_obj["h"],
                            candle_obj["l"],
                            candle_obj["c"],
                            candle_obj["v"]);
        }
        DAWN_INFO(result_);
    }

    void ws_klines_onData(const rapidjson::Document& json_result)
    {
        long start_of_candle = json_result["k"]["t"].GetInt64();
        _klinesCache[start_of_candle]["o"] = json_result["k"]["o"].GetDouble();
        _klinesCache[start_of_candle]["h"] = json_result["k"]["h"].GetDouble();
        _klinesCache[start_of_candle]["l"] = json_result["k"]["l"].GetDouble();
        _klinesCache[start_of_candle]["c"] = json_result["k"]["c"].GetDouble();
        _klinesCache[start_of_candle]["v"] = json_result["k"]["v"].GetDouble();
        print_klinesCache();
    }
};

TEST_F(WebsocketTest, BinanceStreaming)
{
    SA::delegate<void(const rapidjson::Document&)> req_delegate_;
    auto req_delegate_instance_ =
        decltype(req_delegate_)::create<WebsocketTest,
                                        &WebsocketTest::ws_klines_onData>(this);
    const std::string agg_trade = "/ws/bnbbtc@aggTrade";
    Websocket::ConnectEndpoint(
        req_delegate_instance_,
        agg_trade.c_str(),
        std::string(External::BinanceAPI::WebsocketHost).c_str(),
        External::BinanceAPI::WebsocketPort);
    std::thread t(&Websocket::EnterEventLoop);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    Websocket::StopEventLoop();
    t.join();
}

}  // namespace Dawn::Utility