#pragma once

#include <string>
namespace Dawn::Core {

struct TTSConfig {
    TTSConfig() : lang("en") {}
    std::string lang;
};

class TextToSpeechBase {
public:
    virtual std::string ToSpeech(const std::string& text,
                                 TTSConfig&& config = TTSConfig()) const = 0;
};

}  // namespace Dawn::Core