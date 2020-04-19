#include "action.hpp"
#include "utility/general/logging.hpp"

namespace Dawn::Trading {

namespace {
using namespace rapidjson;

}
BinanceKLineData BinanceKLineData::FromJson(
    const GenericValue<UTF8<char>>& doc) {
    BinanceKLineData res;
    DAWN_ENFORCE(doc.IsArray(), "Data is not array");
    DAWN_ENFORCE(doc.Size() >= 11, "Invalid binance kline data");
    res.OpenTime = doc[0].GetInt64();
    res.CloseTime = doc[6].GetInt64();
    res.Open = doc[1].GetDouble();
    res.High = doc[2].GetDouble();
    res.Low = doc[3].GetDouble();
    res.Close = doc[4].GetDouble();
    res.Volume = doc[5].GetDouble();
    res.QuoteAssetVolume = doc[7].GetDouble();
    res.NumberOfTraders = doc[8].GetInt64();
    res.TakerBuyBaseAssetVolume = doc[9].GetDouble();
    res.TakerBuyQuoteAssetVolume = doc[10].GetDouble();
    return res;
}

}  // namespace Dawn::Trading
