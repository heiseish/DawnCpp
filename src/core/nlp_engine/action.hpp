#pragma once

#ifndef CORE_ACTION_H
#define CORE_ACTION_H

#include <vector>

#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

namespace Dawn::Core {

class ActionExecuter {
public:
    virtual ~ActionExecuter() = default;
    virtual std::vector<Message> Execute(MessageRequest&& request) const = 0;
};

}  // namespace Dawn::Core
#endif
