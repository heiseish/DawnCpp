#pragma once

#ifndef DAWN_CORE_PLATFORM_TELEGRAM_H
#define DAWN_CORE_PLATFORM_TELEGRAM_H

#include <libtelegram/libtelegram.h>
#include "core/platform/message.hpp"
#include "core/platform/platform.hpp"
#include "core/platform/user_info.hpp"

namespace Dawn::Core {

class TelegramPlatform : public Platform {
public:
    TelegramPlatform();
    virtual void RegisterHooks() override;
    virtual void Send(const UserInfo& user_info,
                      const Message& message) override;

private:
    telegram::sender _sender;
    telegram::listener::poll _listener;
    const std::string _bot_name;
};

}  // namespace Dawn::Core
#endif
