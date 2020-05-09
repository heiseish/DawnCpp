#include "message.hpp"
#include <any>

namespace Dawn::Core {

bool Message::is(MessageType message_type) const
{
    return message_type == _message_type;
}

bool Message::isText() const { return MessageType::Text == _message_type; }

bool Message::isImage() const { return MessageType::Image == _message_type; }
bool Message::isAudio() const { return MessageType::Audio == _message_type; }
bool Message::isLocation() const
{
    return MessageType::Location == _message_type;
}
bool Message::isVideo() const { return MessageType::Video == _message_type; }

const MessageType& Message::get_type() const { return _message_type; }

}  // namespace Dawn::Core