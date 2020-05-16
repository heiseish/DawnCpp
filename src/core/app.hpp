#pragma once

#include <memory>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "core/nlp_engine/response.hpp"
#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "core/platform/platform.hpp"
#include "core/platform/platform_types.hpp"
#include "core/tts_engine/gtts_rpc_engine.hpp"
#include "utility/general/shared_queue.hpp"

namespace Dawn::Core {

void dawn_signal_handler(int signal_num);

class Application final {
    template <typename T>
    using EventQueue = std::shared_ptr<Utility::SharedQueue<T>>;
    using PlatformMap =
        std::unordered_map<PlatformType, std::unique_ptr<Platform>>;

public:
    Application();
    ~Application();
    bool Initialize();
    void Start();

private:
    void process_request(MessageRequest msg_req);
    void respond(MessageResponse msg_res);
    EventQueue<MessageRequest> _event_queue;
    EventQueue<MessageResponse> _response_queue;
    PlatformMap _platforms;
    TextResponder _responder;
    GttsRPCEngine _tts_engine;
    bool _enable_tts;
};

}  // namespace Dawn::Core
