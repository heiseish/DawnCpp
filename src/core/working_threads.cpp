#include "working_threads.hpp"
#include "utility/network/websocket.hpp"

#ifdef USE_LIBTELEGRAM
#include "core/platform/telegram/tg_platform.hpp"
#endif

namespace Dawn::Core {

// Global variable containing all the threads currently running
std::unique_ptr<EventLoop<MessageResponse, SA::delegate<void(MessageResponse)>>>
    ThreadManager::RespondingThread = nullptr;

std::unique_ptr<EventLoop<MessageRequest, SA::delegate<void(MessageRequest)>>>
    ThreadManager::ProcessingThread = nullptr;

std::vector<std::thread> ThreadManager::PlatformThreads = {};

void ThreadManager::StopAllThreads()
{
    // terminate program
#ifdef USE_LIBTELEGRAM
    DAWN_INFO("Stopping telegram");
    telegram::listener::poll::stop_all();
#endif
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
