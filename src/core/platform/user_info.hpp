#pragma once

#include <string>

namespace Dawn::Core {
struct UserInfo {
    int32_t message_id;
    int64_t user_id;
    std::string first_name;
};

}  // namespace Dawn::Core