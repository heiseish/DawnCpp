#pragma once

#include <memory>
#include <vector>

#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "delegate/Delegate.h"
#include "utility/general/shared_queue.hpp"
#include "utility/general/worker_thread.hpp"

namespace Dawn::Core {

namespace {
template <typename T, typename F>
using EventLoop = Utility::WorkerThread<T, F>;

template <typename T>
using EventThread = std::unique_ptr<EventLoop<T, SA::delegate<void(T)>>>;

}  // namespace

class ThreadManager {
public:
    // Global variable containing all the threads currently running
    static EventThread<MessageResponse> RespondingThread;
    static EventThread<MessageRequest> ProcessingThread;

    static std::vector<std::thread> PlatformThreads;
    static void StopAllThreads();
};

}  // namespace Dawn::Core
