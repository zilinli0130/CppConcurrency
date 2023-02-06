#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename E>
class BlockingQueue
{
private:
    std::mutex _mtx;
    std::condition_variable _signal;
    std::queue<E> _queue;
    int _max_size;

public:
    BlockingQueue(const int& max_size);
    ~BlockingQueue();

    BlockingQueue(const BlockingQueue& q) = delete;
    BlockingQueue& operator==(const BlockingQueue& q) = delete;

public:
    void push(E e);
    E front();
    void pop();
    int size();


};

#endif