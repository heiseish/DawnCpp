#pragma once

#include "message.hpp"
#include "platform_types.hpp"
#include "user_info.hpp"

namespace Dawn::Core {
struct MessageRequest {
    PlatformType platform_type;
    Message message;
    UserInfo user_info;
};

}  // namespace Dawn::Core