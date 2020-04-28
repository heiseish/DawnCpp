#include "message.hpp"
#include <any>

namespace Dawn::Core {

bool Message::is(MessageType message_type)
{
    return message_type == _message_type;
}

const MessageType& Message::get_type() const { return _message_type; }

}  // namespace Dawn::Core