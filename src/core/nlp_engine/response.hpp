#pragma once

#ifndef CORE_TEXT_RESPONDER_H
#define CORE_TEXT_RESPONDER_H

#include <memory>
#include "utility/containers/unordered_map.hpp"

#include "core/nlp_engine/action.hpp"
#include "core/nlp_engine/action/action_types.hpp"
#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "text_classifier.hpp"

namespace Dawn::Core {

class TextResponder {
public:
    TextResponder();
    virtual ~TextResponder() = default;
    virtual MessageResponse GenerateResponse(MessageRequest&& request);

private:
    unordered_map<BaseActionType, std::unique_ptr<ActionExecuter>> _executers;
    TextClassifierEngine _text_classifier;
};

}  // namespace Dawn::Core
#endif