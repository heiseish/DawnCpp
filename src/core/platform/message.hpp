#pragma once

#include <any>

namespace Dawn::Core {
enum MessageType { Text = 0, Image = 1, Audio = 2, Location = 3, Video = 4 };

class Message {
public:
    bool is(MessageType message_type) const;
    bool isText() const;
    bool isImage() const;
    bool isAudio() const;
    bool isLocation() const;
    bool isVideo() const;

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