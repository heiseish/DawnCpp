#include "websocket.hpp"
#include <libwebsockets/lws-service.h>
#include <array>
#include "utility/general/logging.hpp"

namespace Dawn::Utility {

std::atomic<bool> Websocket::_lws_service_cancelled(false);

struct lws_context *Websocket::_context = nullptr;
const char *Websocket::_name = "websocket-api";

std::array<struct lws_protocols, 2> protocols;

void set_protocols()
{
    protocols[0] = {.name = "websocket-api",
                    .callback = Websocket::event_callback,
                    .per_session_data_size = 0,
                    .rx_buffer_size = 65536

    };
    protocols[1] = {
        .name = nullptr,
        .callback = nullptr,
        .per_session_data_size = 0,
        .rx_buffer_size = 0 /* terminator */
    };
}

map<struct lws *, Callback> Websocket::_handles;

int Websocket::event_callback(struct lws *wsi,
                              enum lws_callback_reasons reason,
                              [[maybe_unused]] void *user,
                              void *in,
                              [[maybe_unused]] size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            lws_callback_on_writable(wsi);
            break;

        case LWS_CALLBACK_CLIENT_RECEIVE: {
            // Handle incomming messages here.
            std::string str_result = std::string((char *)in);
            DAWN_INFO(str_result);
            rapidjson::Document json_result;
            if (json_result.Parse(str_result.c_str()).HasParseError()) {
                DAWN_ERROR("Failed to parse json response");
                break;
            }
            if (_handles.find(wsi) != _handles.end())
                _handles[wsi](json_result);
        } break;

        case LWS_CALLBACK_CLIENT_WRITEABLE: {
            DAWN_INFO("LWS_CALLBACK_CLOSED");
            break;
        }
        case LWS_CALLBACK_CLOSED: {
            DAWN_WARN("LWS_CALLBACK_CLOSED");
            break;
        }
        case LWS_CALLBACK_EVENT_WAIT_CANCELLED: {
            DAWN_WARN("LWS_CALLBACK_EVENT_WAIT_CANCELLED");
            break;
        }
        case LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION: {
            DAWN_INFO("LWS_CALLBACK_OPENSSL_PERFORM_SERVER_CERT_VERIFICATION");
            break;
        }
        case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER: {
            DAWN_INFO("LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER");
            break;
        }
        case LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH: {
            DAWN_INFO("LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH");
            break;
        }
        case LWS_CALLBACK_CLIENT_CLOSED: {
            DAWN_INFO("LWS_CALLBACK_CLIENT_CLOSED");
            if (_handles.find(wsi) != _handles.end())
                _handles.erase(wsi);
            atomic_store(&_lws_service_cancelled, true);
            break;
        }
        case LWS_CALLBACK_GET_THREAD_ID: {
            return (uint64_t)pthread_self();
        }
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
            if (_handles.find(wsi) != _handles.end())
                _handles.erase(wsi);
            std::string str_result = std::string((char *)in);
            DAWN_ERROR("LWS_CALLBACK_CLIENT_CONNECTION_ERROR {}", str_result);
            atomic_store(&_lws_service_cancelled, true);
            return -1;
        }
        case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS: {
            DAWN_INFO("LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS");
            return 1;
        }
        case LWS_CALLBACK_LOCK_POLL: {
            DAWN_INFO("LWS_CALLBACK_LOCK_POLL");
            break;
        }
        case LWS_CALLBACK_ADD_POLL_FD: {
            DAWN_INFO("LWS_CALLBACK_ADD_POLL_FD");
            break;
        }
        case LWS_CALLBACK_DEL_POLL_FD: {
            DAWN_INFO("LWS_CALLBACK_DEL_POLL_FD");
            break;
        }
        case LWS_CALLBACK_UNLOCK_POLL: {
            DAWN_INFO("LWS_CALLBACK_UNLOCK_POLL");
            break;
        }
        case LWS_CALLBACK_WSI_CREATE: {
            DAWN_INFO("LWS_CALLBACK_WSI_CREATE");
            break;
        }
        case LWS_CALLBACK_WSI_DESTROY: {
            DAWN_INFO("LWS_CALLBACK_WSI_DESTROY");
            break;
        }
        case LWS_CALLBACK_CHANGE_MODE_POLL_FD: {
            DAWN_INFO("LWS_CALLBACK_CHANGE_MODE_POLL_FD");
            break;
        }
        case LWS_CALLBACK_PROTOCOL_INIT: {
            DAWN_INFO("LWS_CALLBACK_PROTOCOL_INIT");
            break;
        }
        case LWS_CALLBACK_PROTOCOL_DESTROY: {
            DAWN_INFO("LWS_CALLBACK_PROTOCOL_DESTROY");
            break;
        }
        default: {
            DAWN_WARN("Unhandled callback reason = {}", reason);
        }
    }

    return 0;
}

void Websocket::Initialize()
{
    DAWN_INFO("Websocket Initializing");
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    set_protocols();
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols.data();
    info.gid = -1;
    info.uid = -1;

    // This option is needed here to imply LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT
    // option, which must be set on newer versions of OpenSSL.
    info.options = LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT;

    _context = lws_create_context(&info);
    DAWN_INFO("Websocket Initialized");
}

// Register call backs
void Websocket::ConnectEndpoint(Callback user_cb,
                                const char *path,
                                const char *endpoint,
                                const int &port)
{
    // Connect if we are not connected to the server.
    DAWN_INFO("WS Connecting to {}{}", endpoint, path);
    struct lws_client_connect_info ccinfo = {0, .address = endpoint};
    ccinfo.context = _context;
    ccinfo.port = port;
    ccinfo.path = path;
    ccinfo.host = lws_canonical_hostname(_context);
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;
    ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED |
                            LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

    struct lws *conn = lws_client_connect_via_info(&ccinfo);
    _handles[conn] = user_cb;
    DAWN_INFO("Connected");
}

// Entering event loop
void Websocket::EnterEventLoop()
{
    DAWN_INFO("Starting WS Event Loop");
    while (true) {
        lws_service(_context, 500);
        if (_lws_service_cancelled.load()) {
            DAWN_INFO("Flag _lws_service_cancelled is set to true");
            break;
        }
    }
}

///! can also call lws_cancel_service(_context); from other thread
void Websocket::StopEventLoop()
{
    DAWN_INFO("Stopping event loop");
    atomic_store(&_lws_service_cancelled, true);
    if (_context != nullptr) {
        lws_context_destroy(_context);
    }
    _context = nullptr;
}
}  // namespace Dawn::Utility