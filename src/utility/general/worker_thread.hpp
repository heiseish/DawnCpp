// Credit: codeproject
// https://www.codeproject.com/Articles/1169105/Cplusplus-std-thread-Event-Loop-with-Message-Queue
#pragma once

#ifndef DAWN_THREAD_STD_H
#define DAWN_THREAD_STD_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "utility/general/shared_queue.hpp"

namespace Dawn::Utility {

template <class ContentType, class DelegateType>
class WorkerThread {
public:
    /// Constructor
    WorkerThread(const char* threadName,
                 DelegateType callback,
                 std::shared_ptr<Utility::SharedQueue<ContentType>> queue =
                     std::make_shared<Utility::SharedQueue<ContentType>>());

    /// Destructor
    ~WorkerThread();

    /// Called once to create the worker thread
    /// @return TRUE if thread is created. FALSE otherwise.
    bool CreateThread();

    /// Called once a program exit to exit the worker thread
    void ExitThread();

    /// Get the ID of this thread instance
    /// @return The worker thread ID
    std::thread::id GetThreadId();

    /// Get the ID of the currently executing thread
    /// @return The current thread ID
    static std::thread::id GetCurrentThreadId();

    /// Add a message to thread queue.
    /// @param[in] data - thread specific information created on the heap using
    /// operator new.
    void PostMsg(ContentType&& data);

private:
    WorkerThread(const WorkerThread&);
    WorkerThread& operator=(const WorkerThread&);

    /// Entry point for the worker thread
    void Process();

    /// Entry point for timer thread
    void TimerThread();

    DelegateType _callback;
    std::thread* _thread;
    std::shared_ptr<Utility::SharedQueue<ContentType>> _queue;
    const char* _thread_name;
};

//----------------------------------------------------------------------------
// WorkerThread
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
WorkerThread<ContentType, DelegateType>::WorkerThread(
    const char* threadName,
    DelegateType callback,
    std::shared_ptr<Utility::SharedQueue<ContentType>> queue)
    : _callback(callback),
      _thread(nullptr),
      _queue(queue),
      //   _timerExit(false),
      _thread_name(threadName)
{
}

//----------------------------------------------------------------------------
// ~WorkerThread
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
WorkerThread<ContentType, DelegateType>::~WorkerThread()
{
    ExitThread();
}

//----------------------------------------------------------------------------
// CreateThread
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
bool WorkerThread<ContentType, DelegateType>::CreateThread()
{
    if (_thread == nullptr)
        _thread = new std::thread(&WorkerThread::Process, this);
    return true;
}

//----------------------------------------------------------------------------
// GetThreadId
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
std::thread::id WorkerThread<ContentType, DelegateType>::GetThreadId()
{
    DAWN_STRICT_ENFORCE(_thread != nullptr, "Thread not initialized");
    return _thread->get_id();
}

//----------------------------------------------------------------------------
// GetCurrentThreadId
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
std::thread::id WorkerThread<ContentType, DelegateType>::GetCurrentThreadId()
{
    return std::this_thread::get_id();
}

//----------------------------------------------------------------------------
// ExitThread
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
void WorkerThread<ContentType, DelegateType>::ExitThread()
{
    if (nullptr == _thread)
        return;
    ThreadMessage<ContentType> thread_msg_;
    thread_msg_.SetType(ThreadMessageType::Exit);
    // Put exit thread message into the queue
    _queue->push_back(std::move(thread_msg_));
    _thread->join();
    delete _thread;
    _thread = nullptr;
}

//----------------------------------------------------------------------------
// PostMsg
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
void WorkerThread<ContentType, DelegateType>::PostMsg(ContentType&& data)
{
    DAWN_STRICT_ENFORCE(_thread != nullptr, "_thread is not initialized");
    ThreadMessage<ContentType> thread_msg_;
    thread_msg_.SetContent(std::move(data));
    // Add user data msg to queue and notify worker thread
    _queue->push_back(std::move(thread_msg_));
}

//----------------------------------------------------------------------------
// TimerThread
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
void WorkerThread<ContentType, DelegateType>::TimerThread()
{
    // while (!_timerExit) {
    //     // Sleep for 250ms then put a MSG_TIMER message into queue
    //     std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //     ThreadMessage<ContentType> thread_msg_;
    //     thread_msg_.SetType(ThreadMessageType::TimerType);
    //     // Add timer msg to queue and notify worker thread
    //     // std::unique_lock<std::mutex> lk(_mutex);
    //     _queue->push_back(std::move(thread_msg_));
    //     // _cv.notify_one();
    // }
}

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
template <class ContentType, class DelegateType>
void WorkerThread<ContentType, DelegateType>::Process()
{
    // _timerExit = false;
    // std::thread timerThread(&WorkerThread::TimerThread, this);

    while (true) {
        auto msg_ = _queue->get_front();
        switch (msg_.type) {
            case ThreadMessageType::Content: {
                _callback(std::move(*msg_.content));
                break;
            }

            case ThreadMessageType::TimerType:
                DAWN_INFO("Timer expired on {}", _thread_name);
                break;

            case ThreadMessageType::Exit: {
                // _timerExit = true;
                // timerThread.join();
                _queue->clear();
                DAWN_INFO("Exitting thread {}", _thread_name);
                return;
            }

            default:
                break;
        }
    }
}

}  // namespace Dawn::Utility

#endif
