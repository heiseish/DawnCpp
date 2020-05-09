#pragma once

#include <string>
#include "core/nlp_engine/action/action_types.hpp"
namespace Dawn::Core {

/// Simple Text classifier of text string
/// Logic is based on whether `text` contains keywords which indicates the
/// associated intent If multiple intents are found, the one which appear the
/// earliest in `action.hpp` is seleted as main intent
class TextClassifierEngine {
public:
    virtual BaseActionType classify(const std::string& text);
};

}  // namespace Dawn::Core