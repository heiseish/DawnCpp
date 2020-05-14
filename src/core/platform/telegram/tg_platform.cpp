#include "tg_platform.hpp"

#include <string>

#include "core/platform/message.hpp"
#include "core/platform/platform.hpp"
#include "core/platform/platform_types.hpp"
#include "core/platform/user_info.hpp"

#include "utility/general/logging.hpp"
#include "utility/general/timer.hpp"
#include "utility/geography/coordinates.hpp"
#include "utility/string/toint.hpp"

#include "library/algorithm/string/kmp.hpp"
#include "utility/general/env.hpp"
#include "utility/string/replace.hpp"

#include <httplib.h>
#include "fmt/format.h"

namespace Dawn::Core {

TelegramPlatform::TelegramPlatform()
    : _sender(Utility::SafeGetEnv("TELEGRAM_TOKEN")),
      _listener(_sender, 5),
      _bot_name(*(_sender.get_me()->username))
{
    DAWN_INFO("Telegram bot name {}", _bot_name);
}

void TelegramPlatform::RegisterHooks()
{
    DAWN_INFO("Telegram Bot registering hook...");
    _listener.set_callback_message(
        [this](telegram::types::message const& message) mutable {
            std::string text_msg_ = message.text ? *(message.text) : "";
            DAWN_DEBUG("Text message: {}", text_msg_);
            if (message.chat.type == telegram::types::chat::types::GROUP &&
                !text_msg_.empty()) {
                if (!Library::Algorithm::KMPAlgorithm::Contains(text_msg_,
                                                                _bot_name))
                    return;
                text_msg_ = Utility::ReplaceAll(text_msg_, "@" + _bot_name);
                text_msg_ = Utility::ReplaceAll(text_msg_, _bot_name);
            }
            MessageRequest msg_req;
            msg_req.platform_type = PlatformType::Telegram;
            msg_req.user_info.user_id = message.chat.id;
            msg_req.user_info.first_name =
                message.from ? message.from->first_name : "";
            msg_req.user_info.message_id = message.message_id;

            if (message.text) {
                msg_req.message.set<std::string>(MessageType::Text, text_msg_);
            }
            else if (message.location) {
                msg_req.message.set<Geography::GeographicCoordinates>(
                    MessageType::Location,
                    {message.location->latitude, message.location->longitude});
            }
            this->_event_queue->emplace_back(std::move(msg_req));
        });
    _listener.run();
}

void TelegramPlatform::Send(const UserInfo& user_info, const Message& message)
{
    try {
        decltype(auto) res = message.get_type();
        Utility::Timer timer_;
        timer_.Start();
        switch (res) {
            case MessageType::Text:
                _sender.send_message(user_info.user_id,
                                     message.get<std::string>(),
                                     user_info.message_id);
                break;
            case MessageType::Image: {
                telegram::types::file image_file_;
                image_file_.file_id = message.get<std::string>();
                _sender.send_photo(user_info.user_id,
                                   std::move(image_file_),
                                   {},
                                   false,
                                   user_info.message_id);
                break;
            }
            case MessageType::Audio: {
                _sender.send_voice(user_info.user_id,
                                   std::move(message.get<std::string>()),
                                   {},
                                   false,
                                   user_info.message_id);
                break;
            }
            case MessageType::Video: {
                telegram::types::video vide_file_;
                vide_file_.file_id = message.get<std::string>();
                _sender.send_video(user_info.user_id,
                                   std::move(vide_file_),
                                   {},
                                   false,
                                   user_info.message_id);
                break;
            }
            case MessageType::Location: {
                auto res = message.get<Geography::GeographicCoordinates>();
                telegram::types::location location_;
                location_.latitude = res.latitude;
                location_.longitude = res.longitude;
                _sender.send_location(user_info.user_id,
                                      std::move(location_),
                                      {},
                                      false,
                                      user_info.message_id);
            }
        }
        DAWN_DEBUG("[ Telegram ] Send message: {} ms",
                   timer_.Record<MilliSeconds>());
    }
    catch (const std::runtime_error& e) {
        DAWN_ERROR(e.what());
    }
}
}  // namespace Dawn::Core
