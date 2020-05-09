#include "working_threads.hpp"
#include "core/platform/telegram/tg_platform.hpp"
#include "utility/network/websocket.hpp"

namespace Dawn::Core {

// Global variable containing all the threads currently running
EventThread<MessageResponse> ThreadManager::RespondingThread = nullptr;
EventThread<MessageRequest> ThreadManager::ProcessingThread = nullptr;

std::vector<std::thread> ThreadManager::PlatformThreads = {};

void ThreadManager::StopAllThreads()
{
    // terminate program
    DAWN_INFO("Stopping telegram");
    telegram::listener::poll::stop_all();
    DAWN_INFO("Stopping all platform threads");
    for (auto& thread_ : ThreadManager::PlatformThreads)
        thread_.join();
    DAWN_INFO("Stopping processing thread");
    ThreadManager::ProcessingThread->ExitThread();
    DAWN_INFO("Stopping responding thread");
    ThreadManager::RespondingThread->ExitThread();
    // Streaming activity
    DAWN_INFO("Stopping websocket connection");
    Utility::Websocket::StopEventLoop();
}

}  // namespace Dawn::Core
