#pragma once

#include "rapidjson/document.h"

#ifndef DAWN_CORE_TRADING_ACITON_H
#define DAWN_CORE_TRADING_ACITON_H

namespace Dawn::Trading {

enum TraderAction { Buy, Sell };

class TickerData {
public:
    static TickerData FromJson(
        const rapidjson::GenericValue<rapidjson::UTF8<char>>& doc);
};

class BinanceKLineData final : public TickerData {
public:
    static BinanceKLineData FromJson(
        const rapidjson::GenericValue<rapidjson::UTF8<char>>& doc);

    int64_t OpenTime;
    int64_t CloseTime;

    double Open;
    double High;
    double Low;
    double Close;
    double Volume;
    int64_t NumberOfTraders;
    double QuoteAssetVolume;
    double TakerBuyBaseAssetVolume;
    double TakerBuyQuoteAssetVolume;
};

}  // namespace Dawn::Trading

#endif