#pragma once

#include <curl/curl.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace Dawn::External {

class BinanceAPI {
public:
    constexpr static std::string_view WebsocketHost = "stream.binance.com";
    constexpr static int WebsocketPort = 9443;
    constexpr static std::string_view BinanceHost = "https://api.binance.com";

    BinanceAPI(const std::string& api_key, const std::string& secret_key);
    ~BinanceAPI();
    std::string CurlAPI(const std::string& url);
    std::string CurlAPIWithHeader(
        const std::string& url,
        const std::vector<std::string>& extra_http_header,
        const std::string& post_data,
        const std::string& action);
    static size_t CurlCb(void* content,
                         size_t size,
                         size_t nmemb,
                         std::string* buffer);

    void CleanUp();

    // Public API
    std::string GetExchangeInfo();
    std::string GetServerTime();

    std::string GetAllPrices();
    double GetPrice(const std::string_view& symbol);

    std::string GetAllBookTickers();
    std::string GetBookTicker(const std::string_view& symbol);

    std::string GetDepth(const std::string_view& symbol, const int& limit);
    std::string GetAggTrades(const std::string_view& symbol,
                             const int& fromId,
                             const time_t& startTime,
                             const time_t& endTime,
                             const int& limit);
    std::string Get24hr(const std::string_view& symbol);
    std::string GetKLines(const std::string_view& symbol,
                          const std::string_view& interval,
                          const int& limit,
                          const time_t& startTime,
                          const time_t& endTime);

    // API + Secret keys required
    std::string GetAccount(const long& recvWindow);

    std::string GetMyTrades(const std::string_view& symbol,
                            const int& limit,
                            const long& fromId,
                            const long& recvWindow);

    std::string GetOpenOrders(const std::string_view& symbol,
                              const long& recvWindow);

    std::string GetAllOrders(const std::string_view& symbol,
                             const long& orderId,
                             const int& limit,
                             const long& recvWindow);

    std::string SendOrder(const std::string_view& symbol,
                          const std::string_view& side,
                          const std::string_view& type,
                          const std::string_view& timeInForce,
                          const double& quantity,
                          const double& price,
                          const std::string_view& newClientOrderId,
                          const double& stopPrice,
                          const double& icebergQty,
                          const long& recvWindow);

    std::string GetOrder(const std::string_view& symbol,
                         const long& orderId,
                         const std::string_view& origClientOrderId,
                         const long& recvWindow);

    std::string CancelOrder(const std::string_view& symbol,
                            const long& orderId,
                            const std::string_view& origClientOrderId,
                            const std::string_view& newClientOrderId,
                            const long& recvWindow);

    // API key required
    std::string StartUserDataStream();
    void KeepUserDataStream(const std::string_view& listenKey);
    void CloseUserDataStream(const std::string_view& listenKey);

    // WAPI
    std::string Withdraw(const std::string_view& asset,
                         const std::string_view& address,
                         const std::string_view& addressTag,
                         const double& amount,
                         const std::string_view& name,
                         const long& recvWindow);

    std::string GetDepositHistory(const std::string_view& asset,
                                  const int& status,
                                  const long& startTime,
                                  const long& endTime,
                                  const long& recvWindow);

    std::string GetWithdrawHistory(const std::string_view& asset,
                                   const int& status,
                                   const long& startTime,
                                   const long& endTime,
                                   const long& recvWindow);

    std::string GetDepositAddress(const std::string_view& asset,
                                  const long& recvWindow);

private:
    const std::string _api_key;
    const std::string _secret_key;
    CURL* _curl;
};

}  // namespace Dawn::External
