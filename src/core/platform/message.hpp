#pragma once

#ifndef CORE_MESSAGE_H
#define CORE_MESSAGE_H

#include <any>

namespace Dawn::Core {
enum MessageType { Text = 0, Image = 1, Audio = 2, Location = 3, Video = 4 };

class Message {
public:
    Message() = default;
    Message(const Message& other)
        : _message_type(other._message_type), _payload(other._payload)
    {
    }

    Message& operator=(Message& other)
    {
        if (this != &other) {
            _message_type = other._message_type;
            _payload = other._payload;
        }
        return *this;
    }
    Message(Message&& other)
        : _message_type(std::move(other._message_type)),
          _payload(std::move(other._payload))
    {
    }

    Message& operator=(Message&& other)
    {
        if (this != &other) {
            _message_type = std::move(other._message_type);
            _payload = std::move(other._payload);
        }
        return *this;
    }
    bool is(MessageType message_type);
    const MessageType& get_type() const;

    template <typename value_type>
    void set(MessageType message_type, const value_type& val)
    {
        _message_type = message_type;
        _payload = std::make_any<value_type>(val);
    }

    template <typename value_type>
    void set(MessageType message_type, value_type&& val)
    {
        _message_type = message_type;
        _payload = std::make_any<value_type>(std::move(val));
    }

    template <typename value_type>
    value_type get() const
    {
        return std::any_cast<value_type>(_payload);
    }

private:
    MessageType _message_type;
    std::any _payload;
};

}  // namespace Dawn::Core
#endif