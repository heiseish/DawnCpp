#include "app.hpp"
#include <memory>
#include <thread>
#include "core/nlp_engine/response.hpp"
#include "core/platform/platform_types.hpp"
#include "core/platform/telegram/tg_platform.hpp"
#include "delegate/Delegate.h"
#include "utility/general/logging.hpp"
#include "utility/general/timer.hpp"
#include "working_threads.hpp"

namespace Dawn::Core {

void dawn_signal_handler(int signal_num)
{
    DAWN_INFO("Gracefully shutting down with signal ({})", signal_num);
    ThreadManager::StopAllThreads();
    exit(signal_num);
}

Application::Application() {}
Application::~Application() {}

bool Application::Initialize()
{
    try {
        _event_queue = std::make_shared<Utility::SharedQueue<MessageRequest>>();
        _response_queue =
            std::make_shared<Utility::SharedQueue<MessageResponse>>();
        _platforms.emplace(PlatformType::Telegram,
                           std::make_unique<TelegramPlatform>());
        for (const auto& plt : _platforms) {
            plt.second->RegisterEventQueue(_event_queue);
        }
    }
    catch (const std::exception& e) {
        DAWN_ERROR(e.what());
        return false;
    }
    DAWN_INFO("App initialized successfully!");
    return true;
}

void Application::process_request(MessageRequest msg_req)
{
    Utility::Timer timer_;
    timer_.Start();
    _response_queue->emplace_back(
        _responder.GenerateResponse(std::move(msg_req)));
    DAWN_DEBUG("Processing took {} ms", timer_.Record<MilliSeconds>());
}

void Application::respond(MessageResponse msg_res)
{
    Utility::Timer timer_;
    timer_.Start();
    for (const auto& msg : msg_res.message) {
        _platforms.at(msg_res.platform_type)->Send(msg_res.user_info, msg);
    }
    DAWN_DEBUG("Responding took {} ms", timer_.Record<MilliSeconds>());
}

void Application::Start()
{
    for (const auto& platform : _platforms) {
        ThreadManager::PlatformThreads.emplace_back(&Platform::RegisterHooks,
                                                    platform.second.get());
    }

    // ---------- Processing thread -------------
    SA::delegate<void(MessageRequest)> req_delegate_;
    auto req_delegate_instance_ =
        decltype(req_delegate_)::create<Application,
                                        &Application::process_request>(this);
    ThreadManager::ProcessingThread = std::make_unique<
        Utility::WorkerThread<MessageRequest, decltype(req_delegate_)>>(
        "ProcessingThread", req_delegate_instance_, _event_queue);
    ThreadManager::ProcessingThread->CreateThread();

    // ---------- Responding thread -------------
    SA::delegate<void(MessageResponse)> res_delegate_;
    auto res_delegate_instance_ = decltype(
        res_delegate_)::create<Application, &Application::respond>(this);
    ThreadManager::RespondingThread = std::make_unique<
        Utility::WorkerThread<MessageResponse, decltype(res_delegate_)>>(
        "RespondingThread", res_delegate_instance_, _response_queue);
    ThreadManager::RespondingThread->CreateThread();
    while (true) {
    }
}

}  // namespace Dawn::Core
