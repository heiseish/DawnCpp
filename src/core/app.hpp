#pragma once

#ifndef DAWN_CORE_APPLICATION_H
#define DAWN_CORE_APPLICATION_H

#include <memory>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

#include "core/database/database.hpp"
#include "core/nlp_engine/response.hpp"
#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "core/platform/platform.hpp"
#include "core/platform/platform_types.hpp"
#include "trading/trader_manager.hpp"
#include "utility/general/shared_queue.hpp"

namespace Dawn::Core {

void dawn_signal_handler(int signal_num);

class Application final {
public:
    Application();
    ~Application();
    bool Initialize();
    void Start();

private:
    void set_up_cron_jobs();
    void process_request(MessageRequest msg_req);
    void respond(MessageResponse msg_res);
    std::shared_ptr<Utility::SharedQueue<MessageRequest>> _event_queue;
    std::shared_ptr<Utility::SharedQueue<MessageResponse>> _response_queue;
    std::unordered_map<PlatformType, std::unique_ptr<Platform>> _platforms;
    std::shared_ptr<Database> _database;
    TextResponder _responder;
    Trading::TraderManager _trader_manager;
};

}  // namespace Dawn::Core

#endif