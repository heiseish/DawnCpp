#include "unknown.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

#include "fmt/format.h"
#include "protos/py_rpc.grpc.pb.h"
#include "protos/py_rpc.pb.h"

namespace {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using PyRPC::ConversationInput;
using PyRPC::ConversationResponse;
using PyRPC::ConversationResponse_State;
using PyRPC::PyRPCService;

}  // namespace

namespace Dawn::Core {

GPTClient::GPTClient(const std::string server_addr)
{
    _channel =
        grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials());
    _stub = PyRPCService::NewStub(_channel);
}

std::string GPTClient::Respond(const std::string& input_text) const
{
    ConversationInput request;
    request.set_text(input_text);
    ConversationResponse reply;
    // Context for the client. It could be used to convey extra information
    // to the server and/or tweak certain RPC behaviors.
    ClientContext context;
    // The actual RPC.
    Status status = _stub->RespondToText(&context, request, &reply);

    // Act upon its status.
    if (status.ok() &&
        reply.state() ==
            ConversationResponse_State::ConversationResponse_State_SUCCESS) {
        return reply.text();
    }
    DAWN_DEBUG("{}: {}", status.error_code(), status.error_message());
    return "RPC failed";
}

std::vector<Message> UnknownBaseAction::Execute(
    [[maybe_unused]] MessageRequest&& request) const
{
    std::vector<Message> reply;
    Message initial;
    initial.set<std::string>(
        MessageType::Text, _client.Respond(request.message.get<std::string>()));
    reply.emplace_back(std::move(initial));
    return reply;
}

}  // namespace Dawn::Core
