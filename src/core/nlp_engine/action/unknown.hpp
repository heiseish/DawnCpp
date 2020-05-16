#pragma once

#include <string_view>
#include <vector>

#include <grpcpp/grpcpp.h>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"
#include "protos/py_rpc.grpc.pb.h"
#include "protos/py_rpc.pb.h"
#include "utility/general/logging.hpp"

namespace Dawn::Core {

class GPTClient {
public:
    GPTClient(const std::string server_addr = "localhost:8080");
    std::string Respond(const std::string& input_text) const;

private:
    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<PyRPC::PyRPCService::Stub> _stub;
};

class UnknownBaseAction : public ActionExecuter {
public:
    UnknownBaseAction() = default;
    static constexpr std::string_view Name =
        "Action to respond to unknown intent";
    virtual std::vector<Message> Execute(
        MessageRequest&& request) const override;

private:
    GPTClient _client;
};

}  // namespace Dawn::Core
