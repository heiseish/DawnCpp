
#include "gtts_rpc_engine.hpp"
#include <grpcpp/grpcpp.h>
#include <string>
#include "protos/seq2seq_service.grpc.pb.h"
#include "protos/seq2seq_service.pb.h"
#include "utility/general/logging.hpp"

namespace {

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using Text2SpeechRPC::Text2SpeechService;
using Text2SpeechRPC::TTSInput;
using Text2SpeechRPC::TTSOutput;

}  // namespace

namespace Dawn::Core {

GttsClient::GttsClient(const std::string server_addr)
{
    _channel =
        grpc::CreateChannel(server_addr, grpc::InsecureChannelCredentials());
    _stub = Text2SpeechService::NewStub(_channel);
}

std::string GttsClient::TextToSpeech(const std::string& text,
                                     TTSConfig&& config) const
{
    TTSInput request;
    request.set_text(text);
    request.set_lang(config.lang);
    TTSOutput reply;
    ClientContext context;
    Status status = _stub->TextToSpeech(&context, request, &reply);

    if (status.ok()) {
        return reply.data();
    }
    DAWN_ERROR("{}: {}", status.error_code(), status.error_message());
    return "RPC failed";
}

std::string GttsRPCEngine::ToSpeech(const std::string& text,
                                    TTSConfig&& config) const
{
    return _rpc_client.TextToSpeech(text, std::move(config));
}

}  // namespace Dawn::Core