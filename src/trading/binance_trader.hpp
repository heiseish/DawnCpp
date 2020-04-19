#pragma once

#ifndef DAWN_CORE_BINANCE_TRADERS_H
#define DAWN_CORE_BINANCE_TRADERS_H

#include "external/binance.hpp"
#include "trader.hpp"

namespace Dawn::Trading {

class BinanceTraderConfig : public TraderConfig {};

class BinanceTrader : public Trader {
public:
    BinanceTrader(const BinanceTraderConfig& config) noexcept;
    BinanceTrader(BinanceTraderConfig&& config) noexcept;
    ~BinanceTrader();
    virtual void Initialize() override;
    virtual void Start() override;
    virtual void Stop() override;
    void DownloadHistoricalData();

private:
    BinanceTraderConfig _config;
    External::BinanceAPI _binance_api;
    std::string _listen_key;
};

}  // namespace Dawn::Trading

#endif