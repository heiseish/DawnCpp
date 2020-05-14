#include "unknown.hpp"

#include <string>
#include <vector>
#include "core/nlp_engine/action.hpp"
#include "core/platform/message.hpp"
#include "core/platform/message_request.hpp"

#include "fmt/format.h"
#include "protos/seq2seq_service.grpc.pb.h"
#include "protos/seq2seq_service.pb.h"

namespace {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using Seq2SeqGRPC::ConversationInput;
using Seq2SeqGRPC::ConversationResponse;
using Seq2SeqGRPC::ConversationResponse_State;
using Seq2SeqGRPC::Seq2SeqService;

}  // namespace

namespace Dawn::Core {

GPTClient::GPTClient(const std::string server_addr)
{
    _channel =
        grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials());
    _stub = Seq2SeqService::NewStub(_channel);
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

std::vector<Message> UnknownBaseAction::Execute([
    [maybe_unused]] MessageRequest&& request) const
{
    std::vector<Message> reply;
    Message initial;
    initial.set<std::string>(
        MessageType::Text, _client.Respond(request.message.get<std::string>()));
    reply.emplace_back(std::move(initial));
    return reply;
}

}  // namespace Dawn::Core
