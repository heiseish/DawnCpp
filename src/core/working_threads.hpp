#pragma once

#ifndef DAWN_WORKING_THREADS_H
#define DAWN_WORKING_THREADS_H

#include <memory>
#include <vector>

#include "core/platform/message_request.hpp"
#include "core/platform/message_response.hpp"
#include "delegate/Delegate.h"
#include "utility/general/shared_queue.hpp"
#include "utility/general/worker_thread.hpp"

namespace Dawn::Core {

class ThreadManager {
public:
    // Global variable containing all the threads currently running
    static std::unique_ptr<Utility::WorkerThread<
        MessageResponse, SA::delegate<void(MessageResponse)>>>
        RespondingThread;
    static std::unique_ptr<Utility::WorkerThread<
        MessageRequest, SA::delegate<void(MessageRequest)>>>
        ProcessingThread;

    static std::vector<std::thread> PlatformThreads;
    static void StopAllThreads();
};

}  // namespace Dawn::Core

#endif