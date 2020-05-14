#pragma once

#include <grpcpp/grpcpp.h>
#include <string>
#include "base_engine.hpp"
#include "protos/text2speech_service.grpc.pb.h"
#include "protos/text2speech_service.pb.h"

namespace Dawn::Core {

class GttsClient {
public:
    GttsClient(const std::string server_addr = "localhost:8080");
    std::string TextToSpeech(const std::string& text, TTSConfig&& config) const;

private:
    std::shared_ptr<grpc::Channel> _channel;
    std::unique_ptr<Text2SpeechRPC::Text2SpeechService::Stub> _stub;
};

class GttsRPCEngine final : TextToSpeechBase {
public:
    GttsRPCEngine() noexcept = default;
    virtual std::string ToSpeech(
        const std::string& text,
        TTSConfig&& config = TTSConfig()) const override;

private:
    GttsClient _rpc_client;
};

}  // namespace Dawn::Core