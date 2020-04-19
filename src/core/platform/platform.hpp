#pragma once

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#include <memory>

#include "core/database/database.hpp"
#include "core/platform/message.hpp"
#include "core/platform/user_info.hpp"
#include "message_request.hpp"
#include "utility/general/shared_queue.hpp"

namespace Dawn::Core {

class Platform {
public:
    virtual ~Platform() = default;
    void RegisterDatabase(std::shared_ptr<Database> database) {
        _database = database;
    }
    void RegisterEventQueue(
        std::shared_ptr<Utility::SharedQueue<MessageRequest>> event_queue) {
        _event_queue = event_queue;
    }
    virtual void RegisterHooks() = 0;
    virtual void Send(const UserInfo& user_info, const Message& message) = 0;

protected:
    std::shared_ptr<Utility::SharedQueue<MessageRequest>> _event_queue;
    std::shared_ptr<Database> _database;
};

}  // namespace Dawn::Core
#endif