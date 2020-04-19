#pragma once

#ifndef DAWN_EXTERNAL_BINANCE_WEBSOCKET_H
#define DAWN_EXTERNAL_BINANCE_WEBSOCKET_H

#include <libwebsockets.h>
#include <atomic>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include "delegate/Delegate.h"
#include "rapidjson/document.h"
#include "utility/containers/map.hpp"

namespace Dawn::Utility {

using Callback = SA::delegate<void(const rapidjson::Document &)>;

class Websocket final {
public:
    Websocket() = delete;
    static void ConnectEndpoint(Callback user_cb, const char *path,
                                const char *endpoint, const int &port = 9443);
    static void Initialize();
    static void EnterEventLoop();
    static void StopEventLoop();

private:
    friend void set_protocols();
    static int event_callback(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len);
    static const char *_name;
    static map<struct lws *, Callback> _handles;
    static std::atomic<bool> _lws_service_cancelled;
    static struct lws_context *_context;
};

}  // namespace Dawn::Utility

#endif
