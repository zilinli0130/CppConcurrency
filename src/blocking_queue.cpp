#include "../include/blocking_queue.h"
#include <future>

template<class E>
BlockingQueue<E>::BlockingQueue(const int& max_size):
    _max_size(max_size)
{
}

template<class E>
BlockingQueue<E>::~BlockingQueue()
{
}

template<class E>
void BlockingQueue<E>::push(E e)
{
    std::unique_lock<std::mutex> lock(_mtx);
    _signal.wait(lock, [this](){ return _queue.size() < _max_size; });
    _queue.push(e);
    // std::cout << "Produced: " << e << std::endl;
    lock.unlock();
    _signal.notify_one(); 
}

template<class E>
E BlockingQueue<E>::front()
{
    std::unique_lock<std::mutex> lock(_mtx);
    // Atomically unlocks lock, blocks the current executing thread, 
    // and adds it to the list of threads waiting on *this
    _signal.wait(lock, [this](){ return !_queue.empty(); });
    return _queue.front(); 
}

template<class E>
void BlockingQueue<E>::pop()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _signal.wait(lock, [this](){ return !_queue.empty(); });
    // std::cout << "Consumed: " << _queue.front() << std::endl;
    _queue.pop();
    lock.unlock();
    _signal.notify_one();
}

template<class E>
int BlockingQueue<E>::size()
{
    std::lock_guard<std::mutex> lock(_mtx);
    return _queue.size();
}

template class BlockingQueue<int>;
template class BlockingQueue<std::shared_future<int>>;

