#ifndef MSGQUEUE_H
#define MSGQUEUE_H

// #include <iostream>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <thread>

template <class T>
class MessageQueue
{
public:
    T receive();

    void send(T &&msg);
    
    int getSize();

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _queue;
};  

template <typename T>
int MessageQueue<T>::getSize()
{
    std::lock_guard<std::mutex> uLock(_mutex);
    return _queue.size();
}

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> uLock(_mutex);
    // std::cout << "receive starts" << std::endl;
    _cond.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable
    // std::cout << "receive cond met" << std::endl;

    // remove last vector element from queue
    T msg = std::move(_queue.back());
    _queue.pop_back();
    // std::cout << "received" << std::endl;
    return msg; // will not be copied due to return value optimization (RVO) in C++
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // simulate some work
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutex);
    // std::cout << "send" << std::endl;

    // add vector to queue
    _queue.push_back(std::move(msg));
    _cond.notify_one(); // notify client after pushing new Vehicle into vector
}
#endif