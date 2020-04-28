#pragma once

#ifndef CORE_PLATFORM_SQS_H
#define CORE_PLATFORM_SQS_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include "utility/general/logging.hpp"

namespace Dawn::Utility {

enum ThreadMessageType { Content = 0, TimerType, Exit };

template <typename ContentType>
class ThreadMessage {
public:
    void Set(ThreadMessageType type_, ContentType content_);
    template <class... Args>
    void SetContent(Args&&... args);
    void SetType(ThreadMessageType type_);
    bool IsContent() const;
    bool IsTimerType() const;
    bool IsExit() const;
    ThreadMessageType type;
    std::shared_ptr<ContentType> content{nullptr};
};

template <typename ContentType>
template <class... Args>
void ThreadMessage<ContentType>::SetContent(Args&&... args)
{
    type = ThreadMessageType::Content;
    content = std::make_shared<ContentType>(std::forward<Args>(args)...);
}

template <typename ContentType>
void ThreadMessage<ContentType>::SetType(ThreadMessageType type_)
{
    type = type_;
}

template <typename ContentType>
void ThreadMessage<ContentType>::Set(ThreadMessageType type_,
                                     ContentType content_)
{
    type = type_;
    *content = content_;
}

template <typename ContentType>
bool ThreadMessage<ContentType>::IsContent() const
{
    return type == ThreadMessageType::Content;
}

template <typename ContentType>
bool ThreadMessage<ContentType>::IsTimerType() const
{
    return type == ThreadMessageType::TimerType;
}

template <typename ContentType>
bool ThreadMessage<ContentType>::IsExit() const
{
    return type == ThreadMessageType::Exit;
}
/// Synchronized and thread safe queue used for polling
template <typename T>
class SharedQueue {
public:
    SharedQueue();
    ~SharedQueue();

    ThreadMessage<T>& front();
    void pop_front();
    ThreadMessage<T> get_front();

    void push_back(const ThreadMessage<T>& item);
    void push_back(ThreadMessage<T>&& item);

    template <class... Args>
    void emplace_back(Args&&... args);

    void clear();
    int size();
    bool empty();

private:
    std::deque<ThreadMessage<T>> _queue;
    std::mutex _mutex;
    std::condition_variable _cond;
};

template <typename T>
SharedQueue<T>::SharedQueue()
{
}

template <typename T>
SharedQueue<T>::~SharedQueue()
{
}

template <typename T>
ThreadMessage<T>& SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_queue.empty()) {
        _cond.wait(mlock);
    }
    return _queue.front();
}

template <typename T>
void SharedQueue<T>::pop_front()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_queue.empty()) {
        _cond.wait(mlock);
    }
    _queue.pop_front();
}

template <typename T>
ThreadMessage<T> SharedQueue<T>::get_front()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    while (_queue.empty()) {
        _cond.wait(mlock);
    }
    auto val = _queue.front();
    _queue.pop_front();
    return val;
}

template <typename T>
void SharedQueue<T>::push_back(const ThreadMessage<T>& item)
{
    std::unique_lock<std::mutex> mlock(_mutex);
    _queue.push_back(item);
    mlock.unlock();      // unlock before notificiation to minimize mutex con
    _cond.notify_one();  // notify one waiting thread
}

template <typename T>
void SharedQueue<T>::push_back(ThreadMessage<T>&& item)
{
    std::unique_lock<std::mutex> mlock(_mutex);
    _queue.push_back(std::move(item));
    mlock.unlock();      // unlock before notificiation to minimize mutex con
    _cond.notify_one();  // notify one waiting thread
}

template <typename T>
template <class... Args>
void SharedQueue<T>::emplace_back(Args&&... args)
{
    std::unique_lock<std::mutex> mlock(_mutex);
    ThreadMessage<T> thread_msg_;
    thread_msg_.SetContent(std::forward<Args>(args)...);
    _queue.emplace_back(std::move(thread_msg_));
    mlock.unlock();      // unlock before notificiation to minimize mutex con
    _cond.notify_one();  // notify one waiting thread
}

template <typename T>
int SharedQueue<T>::size()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    int size = _queue.size();
    mlock.unlock();
    return size;
}

template <typename T>
void SharedQueue<T>::clear()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    while (!_queue.empty())
        _queue.pop_front();
}

template <typename T>
bool SharedQueue<T>::empty()
{
    std::unique_lock<std::mutex> mlock(_mutex);
    return _queue.empty();
}

}  // namespace Dawn::Utility
#endif