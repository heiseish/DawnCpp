#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "utility/containers/unordered_map.hpp"

#include "core/nlp_engine/action.hpp"
#include "core/nlp_engine/action/action_types.hpp"
#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "text_classifier.hpp"

namespace Dawn::Core {

class TextResponder {
    using ActionCallback = std::function<std::vector<Message>(MessageRequest)>;

public:
    TextResponder();
    virtual ~TextResponder() = default;
    virtual MessageResponse GenerateResponse(MessageRequest&& request);

private:
    unordered_map<BaseActionType, ActionCallback> _mapper;
    std::vector<std::unique_ptr<ActionExecuter>> _executers;
    TextClassifierEngine _text_classifier;
};

}  // namespace Dawn::Core