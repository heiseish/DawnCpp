#pragma once

#include <vector>

#include "message.hpp"
#include "platform_types.hpp"
#include "user_info.hpp"

namespace Dawn::Core {
struct MessageResponse {
    PlatformType platform_type;
    std::vector<Message> message;
    UserInfo user_info;
};

}  // namespace Dawn::Core