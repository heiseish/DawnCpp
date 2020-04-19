#include "binance.hpp"
#include <string_view>
#include "delegate/Delegate.h"
#include "fmt/format.h"
#include "rapidjson/document.h"
#include "utility/crypto/encrypt.hpp"
#include "utility/general/logging.hpp"
#include "utility/general/timer.hpp"
#include "utility/string/toupper.hpp"

namespace Dawn::External {

//---------------------------------
BinanceAPI::BinanceAPI(const std::string &api_key,
                       const std::string &secret_key)
    : _api_key(api_key), _secret_key(secret_key), _curl(curl_easy_init()) {}

BinanceAPI::~BinanceAPI() { CleanUp(); }
void BinanceAPI::CleanUp() {
    curl_easy_cleanup(_curl);
    curl_global_cleanup();
}

//------------------
// GET api/v1/exchangeInfo
//------------------
std::string BinanceAPI::GetExchangeInfo() {
    DAWN_INFO("<BinanceAPI::get_exchangeInfo>");
    auto url = fmt::format("{}/api/v1/exchangeInfo", BinanceAPI::BinanceHost);
    return CurlAPI(url);
}
//------------------
// GET /api/v1/time
//------------
std::string BinanceAPI::GetServerTime() {
    DAWN_INFO("<BinanceAPI::get_serverTime>");
    auto url = fmt::format("{}/api/v1/time", BinanceAPI::BinanceHost);
    return CurlAPI(url);
}

//--------------------
// Get Latest price for all symbols.
/*
        GET /api/v1/ticker/allPrices
*/
std::string BinanceAPI::GetAllPrices() {
    DAWN_INFO("<BinanceAPI::get_allPrices>");
    auto url =
        fmt::format("{}/api/v1/ticker/allPrices", BinanceAPI::BinanceHost);
    return CurlAPI(url);
}

//----------
// Get Single Pair's Price
double BinanceAPI::GetPrice(const std::string_view &symbol) {
    DAWN_INFO("<BinanceAPI::get_price>");
    std::string str_symbol(symbol);
    Utility::ToUpper(str_symbol);
    auto alltickers = GetAllPrices();
    if (alltickers.empty()) return 0.0;
    rapidjson::Document d;
    if (d.Parse(alltickers.c_str()).HasParseError()) {
        DAWN_ERROR("Failed to parse json response");
        return 0.0;
    }
    DAWN_STRICT_ENFORCE(d.IsArray(), "Data is not array");
    for (rapidjson::SizeType i = 0; i < d.Size(); ++i) {
        if (d[i]["symbol"].GetString() == str_symbol) {
            return d[i]["price"].GetDouble();
        }
    }
    return 0.0;
}

//--------------------
// Get Best price/qty on the order book for all symbols.
/*
        GET /api/v1/ticker/allBookTickers

*/

std::string BinanceAPI::GetAllBookTickers() {
    DAWN_INFO("<BinanceAPI::GetAllBookTickers>");
    auto url =
        fmt::format("{}/api/v1/ticker/allBookTickers", BinanceAPI::BinanceHost);
    return CurlAPI(url);
}

//--------------
std::string BinanceAPI::GetBookTicker(const std::string_view &symbol) {
    DAWN_INFO("<BinanceAPI::GetBookTicker>");
    std::string str_symbol(symbol);
    Utility::ToUpper(str_symbol);
    auto alltickers = GetAllBookTickers();
    if (alltickers.empty()) return {};
    rapidjson::Document d;
    if (d.Parse(alltickers.c_str()).HasParseError()) {
        DAWN_ERROR("Failed to parse json response");
        return {};
    }
    DAWN_ENFORCE(d.IsArray(), "Data is not array");
    for (rapidjson::SizeType i = 0; i < d.Size(); ++i) {
        if (d[i]["symbol"].GetString() == str_symbol) {
            return d[i].GetString();
        }
    }
    return {};
}

//--------------------
// Get Market Depth
/*
GET /api/v1/depth

Name	Type		Mandatory	Description
symbol	std::STRING		YES
limit	INT		NO		Default 100; max 100.

*/

std::string BinanceAPI::GetDepth(const std::string_view &symbol,
                                 const int &limit) {
    DAWN_INFO("<BinanceAPI::get_depth>");
    auto url = fmt::format("{}/api/v1/depth?symbol={}&limit={}",
                           BinanceAPI::BinanceHost, symbol, limit);
    return CurlAPI(url);
}

//--------------------
// Get Aggregated Trades list
/*

GET /api/v1/aggTrades

Name		Type	Mandatory	Description
symbol		std::STRING	YES
fromId		LONG	NO		ID to get aggregate trades from
INCLUSIVE. startTime	LONG	NO		Timestamp in ms to get aggregate
trades from INCLUSIVE.
endTime		LONG	NO		Timestamp in ms to get aggregate trades
until INCLUSIVE. limit		INT	NO		Default 500; max 500.
*/

std::string BinanceAPI::GetAggTrades(const std::string_view &symbol,
                                     const int &fromId, const time_t &startTime,
                                     const time_t &endTime, const int &limit) {
    DAWN_INFO("<BinanceAPI::get_aggTrades>");
    auto url = fmt::format("{}/api/v1/aggTrades?symbol={}",
                           BinanceAPI::BinanceHost, symbol);
    // return CurlAPI(url);
    if (startTime != 0 && endTime != 0) {
        url =
            fmt::format("{}&startTime={}&endTime={}", url, startTime, endTime);
    } else {
        url = fmt::format("{}&fromId={}&limit={}", url, fromId, limit);
    }
    DAWN_INFO("<BinanceAPI::get_aggTrades> url = |{}|", url);
    return CurlAPI(url);
}

//--------------------
// Get 24hr ticker price change statistics
/*
Name	Type	Mandatory	Description
symbol	std::STRING	YES
*/
std::string BinanceAPI::Get24hr(const std::string_view &symbol) {
    DAWN_INFO("<BinanceAPI::get_24hr>");
    auto url = fmt::format("{}/api/v1/ticker/24hr?symbol={}",
                           BinanceAPI::BinanceHost, symbol);

    DAWN_INFO("<BinanceAPI::get_24hr> url = |{}|", url);
    return CurlAPI(url);
}

//-----------------
/*

Get KLines( Candle stick / OHLC )
GET /api/v1/klines

Name		Type	Mandatory	Description
symbol		std::STRING	YES
interval	ENUM	YES
limit		INT		NO	Default 500; max 500.
startTime	LONG	NO
endTime		LONG	NO

*/

std::string BinanceAPI::GetKLines(const std::string_view &symbol,
                                  const std::string_view &interval,
                                  const int &limit, const time_t &startTime,
                                  const time_t &endTime) {
    DAWN_INFO("<BinanceAPI::get_klines>");
    auto url = fmt::format("{}/api/v1/klines?symbol={}&interval={}",
                           BinanceAPI::BinanceHost, symbol, interval);
    if (startTime > 0 && endTime > 0) {
        url =
            fmt::format("{}&startTime={}&endTime={}", url, startTime, endTime);
    } else if (limit > 0) {
        url = fmt::format("{}&limit={}", url, limit);
    }
    DAWN_INFO("<BinanceAPI::get_klines> url = |{}|", url);
    return CurlAPI(url);
}

//--------------------
// Get current account information. (SIGNED)
/*
GET /api/v3/account

Parameters:
Name		Type	Mandatory	Description
recvWindow	LONG	NO
timestamp	LONG	YES
*/

std::string BinanceAPI::GetAccount(const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_account>");
    if (_api_key.empty() || _secret_key.empty()) {
        DAWN_ERROR("API Key and Secret Key has not been set.");
        return {};
    }
    auto url = fmt::format("{}/api/v3/account?", BinanceAPI::BinanceHost);
    std::string querystring =
        fmt::format("timestamp={}", Utility::GetCurrentEpochMs());
    if (recvWindow > 0) {
        querystring = fmt::format("{}&recvWindow={}", querystring, recvWindow);
    }
    std::string action = "GET";
    auto signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    url = fmt::format("{}{}&signature={}", url, querystring, signature);
    std::vector<std::string> extra_http_header;
    auto header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(header_chunk);
    DAWN_INFO("<BinanceAPI::get_account> url = |{}|", url);
    return CurlAPIWithHeader(url, extra_http_header, "", action);
}

//--------------------
// Get trades for a specific account and symbol. (SIGNED)
/*
GET /api/v3/myTrades
Name		Type	Mandatory	Description
symbol		std::STRING	YES
limit		INT		NO	Default 500; max 500.
fromId		LONG	NO	TradeId to fetch from. Default gets most recent
trades. recvWindow	LONG	NO timestamp	LONG	YES

*/

std::string BinanceAPI::GetMyTrades(const std::string_view &symbol,
                                    const int &limit, const long &fromId,
                                    const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_myTrades>");
    if (_api_key.empty() || _secret_key.empty()) {
        DAWN_ERROR("API Key and Secret Key has not been set.");
        return {};
    }
    auto url = fmt::format("{}/api/v3/myTrades?", BinanceAPI::BinanceHost);
    std::string querystring = fmt::format("symbol={}", symbol);
    if (limit > 0) {
        querystring = fmt::format("{}&limit={}", querystring, limit);
    }

    if (fromId > 0) {
        querystring = fmt::format("{}&fromId={}", querystring, fromId);
    }

    if (recvWindow > 0) {
        querystring = fmt::format("{}&recvWindow={}", querystring, recvWindow);
    }
    querystring = fmt::format("{}&timestamp={}", querystring,
                              Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    url = fmt::format("{}{}&signature={}", url, querystring, signature);
    std::vector<std::string> extra_http_header{
        fmt::format("X-MBX-APIKEY: {}", _api_key)};
    DAWN_INFO("<BinanceAPI::get_myTrades> url = |{}|", url);
    return CurlAPIWithHeader(url, extra_http_header, "", "GET");
}

//--------------------
// Open Orders (SIGNED)
/*
GET /api/v3/openOrders

Name		Type	Mandatory	Description
symbol		std::STRING	YES
recvWindow	LONG	NO
timestamp	LONG	YES
*/

std::string BinanceAPI::GetOpenOrders(const std::string_view &symbol,
                                      const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_openOrders>");
    if (_api_key.empty() || _secret_key.empty()) {
        DAWN_ERROR("API Key and Secret Key has not been set.");
        return {};
    }

    auto url = fmt::format("{}/api/v3/openOrders?", BinanceAPI::BinanceHost);

    std::string querystring = fmt::format("symbol={}", symbol);
    if (recvWindow > 0) {
        querystring = fmt::format("{}&recvWindow={}", querystring, recvWindow);
    }
    querystring = fmt::format("{}&timestamp={}", querystring,
                              Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring = fmt::format("{}&signature={}", querystring, signature);
    url = fmt::format("{}{}", url, querystring);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.push_back(header_chunk);
    DAWN_INFO("<BinanceAPI::get_openOrders> url = |{}|", url);
    return CurlAPIWithHeader(url, extra_http_header, "", "GET");
}

//--------------------
// All Orders (SIGNED)
/*
GET /api/v3/allOrders

Name		Type	Mandatory	Description
symbol		std::STRING	YES
orderId		LONG	NO
limit		INT		NO		Default 500; max 500.
recvWindow	LONG	NO
timestamp	LONG	YES
*/

std::string BinanceAPI::GetAllOrders(const std::string_view &symbol,
                                     const long &orderId, const int &limit,
                                     const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_allOrders>");
    if (_api_key.empty() || _secret_key.empty()) {
        DAWN_ERROR("API Key and Secret Key has not been set.");
        return {};
    }
    auto url = fmt::format("{}/api/v3/allOrders?", BinanceAPI::BinanceHost);
    std::string querystring = fmt::format("symbol={}", symbol);
    if (orderId > 0) {
        querystring += fmt::format("&orderId={}", orderId);
    }
    if (limit > 0) {
        querystring += fmt::format("&limit={}", limit);
    }
    if (recvWindow > 0) {
        querystring += fmt::format("&recvWindow={}", recvWindow);
    }
    querystring += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring += fmt::format("&signature={}", std::move(signature));
    url += std::move(querystring);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    DAWN_INFO("<BinanceAPI::get_allOrders> url = |{}|", url);
    std::string post_data = "";
    return CurlAPIWithHeader(url, extra_http_header, post_data, "GET");
}

//------------
/*
send order (SIGNED)
POST /api/v3/order

Name				Type		Mandatory	Description
symbol				std::STRING		YES
side				ENUM		YES
type				ENUM		YES
timeInForce			ENUM		YES
quantity			DECIMAL		YES
price				DECIMAL		YES
newClientOrderId		std::STRING		NO		A unique
id for the order. Automatically generated by default.
stopPrice			DECIMAL		NO		Used with STOP
orders
icebergQty			DECIMAL		NO		Used with
icebergOrders recvWindow			LONG		NO timestamp
LONG		YES
*/

std::string BinanceAPI::SendOrder(
    const std::string_view &symbol, const std::string_view &side,
    const std::string_view &type, const std::string_view &timeInForce,
    const double &quantity, const double &price,
    const std::string_view &newClientOrderId, const double &stopPrice,
    const double &icebergQty, const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::send_order>");
    if (_api_key.empty() || _secret_key.empty()) {
        DAWN_ERROR("API Key and Secret Key has not been set.");
        return {};
    }
    auto url = fmt::format("{}/api/v3/order?", BinanceAPI::BinanceHost);
    std::string action = "POST";
    std::string post_data =
        fmt::format("symbol={}&side={}&type={}", symbol, side, type);
    if (type != "MARKET") {
        post_data +=
            fmt::format("&timeInForce={}&price={}", timeInForce, price);
    }
    post_data += fmt::format("&quantity={}", quantity);

    if (newClientOrderId.length() > 0) {
        post_data += fmt::format("&newClientOrderId={}", newClientOrderId);
    }

    if (stopPrice > 0.0) {
        post_data += fmt::format("&stopPrice={}", stopPrice);
    }

    if (icebergQty > 0.0) {
        post_data += fmt::format("&icebergQty={}", icebergQty);
    }

    if (recvWindow > 0) {
        post_data += fmt::format("&recvWindow={}", recvWindow);
    }
    post_data += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());

    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), post_data.c_str());
    post_data += fmt::format("&signature={}", signature);

    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));

    DAWN_INFO("<BinanceAPI::send_order> url = |{}|, post_data = |{}|",
              url.c_str(), post_data.c_str());

    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//------------------
/*
// get order (SIGNED)
GET /api/v3/order

Name				Type	Mandatory	Description
symbol				std::STRING	YES
orderId				LONG	NO
origClientOrderId		std::STRING	NO
recvWindow			LONG	NO
timestamp			LONG	YES
*/

std::string BinanceAPI::GetOrder(const std::string_view &symbol,
                                 const long &orderId,
                                 const std::string_view &origClientOrderId,
                                 const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::GetOrder>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");
    auto url = fmt::format("{}/api/v3/order?", BinanceAPI::BinanceHost);

    std::string querystring = fmt::format("symbol={}", symbol);
    if (orderId > 0) {
        querystring += fmt::format("orderId={}", orderId);
    }

    if (origClientOrderId.length() > 0) {
        querystring += fmt::format("origClientOrderId={}", origClientOrderId);
    }

    if (recvWindow > 0) {
        querystring += fmt::format("recvWindow={}", recvWindow);
    }
    querystring += fmt::format("timestamp={}", Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring += fmt::format("signature={}", signature);

    url += querystring;

    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));

    std::string post_data = "";
    std::string action = "GET";
    DAWN_INFO("<BinanceAPI::GetOrder> url = |{}|", url.c_str());

    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//------------
/*
DELETE /api/v3/order
cancel order (SIGNED)

symbol				std::STRING	YES
orderId				LONG	NO
origClientOrderId		std::STRING	NO
newClientOrderId		std::STRING	NO	Used to uniquely
identify this cancel. Automatically generated by default. recvWindow
LONG NO timestamp			LONG	YES

*/

std::string BinanceAPI::CancelOrder(const std::string_view &symbol,
                                    const long &orderId,
                                    const std::string_view &origClientOrderId,
                                    const std::string_view &newClientOrderId,
                                    const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::send_order>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");

    auto url = fmt::format("{}/api/v3/order?", BinanceAPI::BinanceHost);

    std::string action = "DELETE";

    std::string post_data = fmt::format("symbol={}", symbol);

    if (orderId > 0) {
        post_data += fmt::format("&orderId={}", orderId);
    }

    if (origClientOrderId.length() > 0) {
        post_data += fmt::format("&origClientOrderId={}", origClientOrderId);
    }

    if (newClientOrderId.length() > 0) {
        post_data += fmt::format("&newClientOrderId={}", newClientOrderId);
    }

    if (recvWindow > 0) {
        post_data += fmt::format("&recvWindow={}", recvWindow);
    }
    post_data += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());

    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), post_data.c_str());
    post_data += fmt::format("&signature={}", signature);

    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));

    DAWN_INFO("<BinanceAPI::SendOrder> url = |{}|, post_data = |{}|",
              url.c_str(), post_data.c_str());

    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//--------------------
// Start user data stream (API-KEY)

std::string BinanceAPI::StartUserDataStream() {
    DAWN_INFO("<BinanceAPI::start_userDataStream>");
    DAWN_ENFORCE(!_api_key.empty(), "API Key has not been set.");
    auto url = fmt::format("{}/api/v1/userDataStream", BinanceAPI::BinanceHost);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    DAWN_INFO("<BinanceAPI::start_userDataStream> url = |{}|", url.c_str());
    std::string action = "POST";
    std::string post_data = "";
    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//--------------------
// Keepalive user data stream (API-KEY)
void BinanceAPI::KeepUserDataStream(const std::string_view &listenKey) {
    DAWN_INFO("<BinanceAPI::keep_userDataStream>");
    if (_api_key.empty()) {
        DAWN_ERROR("API key has not been set");
        return;
    }
    auto url = fmt::format("{}/api/v1/userDataStream", BinanceAPI::BinanceHost);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    std::string action = "PUT";
    std::string post_data = fmt::format("listenKey={}", listenKey);
    DAWN_INFO("<BinanceAPI::keep_userDataStream> url = |{}|, post_data = |{}|",
              url, post_data);
    CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//--------------------
// Keepalive user data stream (API-KEY)
void BinanceAPI::CloseUserDataStream(const std::string_view &listenKey) {
    DAWN_INFO("<BinanceAPI::close_userDataStream>");
    if (_api_key.empty()) {
        DAWN_ERROR("API Key has not been set.");
        return;
    };
    auto url = fmt::format("{}/api/v1/userDataStream", BinanceAPI::BinanceHost);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));

    std::string action = "DELETE";
    std::string post_data = fmt::format("listenKey={}", listenKey);

    DAWN_INFO("<BinanceAPI::close_userDataStream> url = |{}|, post_data = |{}|",
              url.c_str(), post_data.c_str());

    CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//-------------
/*
Submit a withdraw request.

POST /wapi/v3/withdraw.html

Name		Type	Mandatory	Description
asset		std::STRING	YES
address		std::STRING	YES
addressTag	std::STRING	NO	Secondary address identifier for coins
like
XRP,XMR etc. amount		DECIMAL	YES name std::STRING	NO Description
of the address recvWindow	LONG	NO timestamp	LONG	YES

*/
std::string BinanceAPI::Withdraw(const std::string_view &asset,
                                 const std::string_view &address,
                                 const std::string_view &addressTag,
                                 const double &amount,
                                 const std::string_view &name,
                                 const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::withdraw>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");
    auto url = fmt::format("{}/wapi/v3/withdraw.html", BinanceAPI::BinanceHost);
    std::string action = "POST";
    std::string post_data = fmt::format("asset={}&address=", asset, address);

    if (!addressTag.empty()) {
        post_data += fmt::format("&addressTag={}", addressTag);
    }
    post_data += fmt::format("&amount={}", amount);

    if (!name.empty()) {
        post_data += fmt::format("&name={}", name);
    }

    if (recvWindow > 0) {
        post_data += fmt::format("&recvWindow={}", recvWindow);
    }
    post_data += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), post_data.c_str());
    post_data += fmt::format("&signature={}", signature);
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));

    DAWN_INFO("<BinanceAPI::withdraw> url = |{}|, post_data = |{}|", url,
              post_data);
    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

/*
-GET /wapi/v3/depositHistory.html
Fetch deposit history.

Parameters:

Name		Type	Mandatory	Description
asset		std::STRING	NO
status		INT	NO	0(0:pending,1:success)
startTime	LONG	NO
endTime	LONG		NO
recvWindow	LONG	NO
timestamp	LONG	YES
*/
std::string BinanceAPI::GetDepositHistory(const std::string_view &asset,
                                          const int &status,
                                          const long &startTime,
                                          const long &endTime,
                                          const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_depostHistory>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");
    auto url =
        fmt::format("{}/wapi/v3/depositHistory.html?", BinanceAPI::BinanceHost);
    std::string querystring = "";

    if (!asset.empty()) {
        querystring += fmt::format("asset={}", asset);
    }

    if (status > 0) {
        querystring += fmt::format("&status={}", status);
    }

    if (startTime > 0) {
        querystring += fmt::format("&startTime={}", startTime);
    }

    if (endTime > 0) {
        querystring += fmt::format("&endTime={}", endTime);
    }

    if (recvWindow > 0) {
        querystring += fmt::format("&recvWindow={}", recvWindow);
    }
    querystring += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());

    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring += fmt::format("&signature={}", signature);
    url += querystring;
    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    std::string post_data = "";
    DAWN_INFO("<BinanceAPI::get_depostHistory> url = |{}|", url.c_str());
    std::string action = "GET";
    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//---------

/*
-GET /wapi/v3/withdrawHistory.html
Fetch withdraw history.

Parameters:

Name		Type	Mandatory	Description
asset		std::STRING	NO
status		INT	NO	0(0:Email Sent,1:Cancelled 2:Awaiting Approval
3:Rejected 4:Processing 5:Failure 6Completed) startTime	LONG	NO endTime
LONG		NO recvWindow	LONG	NO timestamp	LONG	YES
*/

std::string BinanceAPI::GetWithdrawHistory(const std::string_view &asset,
                                           const int &status,
                                           const long &startTime,
                                           const long &endTime,
                                           const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_withdrawHistory>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");
    auto url = fmt::format("{}/wapi/v3/withdrawHistory.html?",
                           BinanceAPI::BinanceHost);
    std::string action = "GET";
    std::string querystring = "";
    if (!asset.empty()) {
        querystring += fmt::format("asset={}", asset);
    }

    if (status > 0) {
        querystring += fmt::format("&status={}", status);
    }

    if (startTime > 0) {
        querystring += fmt::format("&startTime={}", startTime);
    }

    if (endTime > 0) {
        querystring += fmt::format("&endTime={}", endTime);
    }

    if (recvWindow > 0) {
        querystring += fmt::format("&recvWindow={}", recvWindow);
    }
    querystring += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());

    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring += fmt::format("&signature={}", signature);
    url += querystring;

    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    std::string post_data = "";
    DAWN_INFO("<BinanceAPI::get_withdrawHistory> url = |{}|", url.c_str());
    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//--------------
/*
-GET /wapi/v3/depositAddress.html
Fetch deposit address.

Parameters:

Name		Type	Mandatory	Description
asset		std::STRING	YES
recvWindow	LONG	NO
timestamp	LONG	YES

*/

std::string BinanceAPI::GetDepositAddress(const std::string_view &asset,
                                          const long &recvWindow) {
    DAWN_INFO("<BinanceAPI::get_depositAddress>");
    DAWN_ENFORCE(!_api_key.empty() && !_secret_key.empty(),
                 "API Key and Secret Key has not been set.");
    auto url =
        fmt::format("{}/wapi/v3/depositAddress.html?", BinanceAPI::BinanceHost);
    const std::string action = "GET";

    std::string querystring = fmt::format("asset={}", asset);

    if (recvWindow > 0) {
        querystring += fmt::format("&recvWindow={}", recvWindow);
    }
    querystring += fmt::format("&timestamp={}", Utility::GetCurrentEpochMs());
    std::string signature =
        Utility::hmac_sha256(_secret_key.c_str(), querystring.c_str());
    querystring += fmt::format("&signature={}", signature);
    url += querystring;

    std::vector<std::string> extra_http_header;
    std::string header_chunk = fmt::format("X-MBX-APIKEY: {}", _api_key);
    extra_http_header.emplace_back(std::move(header_chunk));
    std::string post_data = "";
    DAWN_INFO("<BinanceAPI::get_depositAddress> url = |%s|", url.c_str());
    return CurlAPIWithHeader(url, extra_http_header, post_data, action);
}

//-----------------
// Curl's callback
size_t BinanceAPI::CurlCb(void *content, size_t size, size_t nmemb,
                          std::string *buffer) {
    DAWN_INFO("<BinanceAPI::curl_cb> ");
    buffer->append((char *)content, size * nmemb);
    DAWN_INFO("<BinanceAPI::curl_cb> done");
    return size * nmemb;
}

//--------------------------------------------------
std::string BinanceAPI::CurlAPI(const std::string &url) {
    std::vector<std::string> v;
    std::string action = "GET";
    std::string post_data = "";
    return CurlAPIWithHeader(url, v, post_data, action);
}

//--------------------
// Do the curl
std::string BinanceAPI::CurlAPIWithHeader(
    const std::string &url, const std::vector<std::string> &extra_http_header,
    const std::string &post_data, const std::string &action) {
    DAWN_INFO("<BinanceAPI::curl_api>");
    CURLcode res;
    if (_curl == nullptr) {
        return {};
    }
    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &BinanceAPI::CurlCb);
    std::string result_;
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &result_);
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(_curl, CURLOPT_ENCODING, "gzip");

    if (extra_http_header.size() > 0) {
        struct curl_slist *chunk = NULL;
        for (unsigned int i = 0; i < extra_http_header.size(); ++i) {
            chunk = curl_slist_append(chunk, extra_http_header[i].c_str());
        }
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, chunk);
    }
    if (post_data.size() > 0 || action == "POST" || action == "PUT" ||
        action == "DELETE") {
        if (action == "PUT" || action == "DELETE") {
            curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, action.c_str());
        }
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, post_data.c_str());
    }
    res = curl_easy_perform(_curl);
    /* Check for errors */
    if (res != CURLE_OK) {
        DAWN_ERROR("<BinanceAPI::curl_api> curl_easy_perform() failed: %s",
                   curl_easy_strerror(res));
        return {};
    }
    return result_;
}

}  // namespace Dawn::External