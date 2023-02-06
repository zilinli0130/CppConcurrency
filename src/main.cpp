#include <iostream>
#include <future>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <queue>
#include "../include/blocking_queue.h"

using namespace std;

namespace {
std::mutex global_mtx;
const int MAX_THREAD_NUMBER = 2;
const int THREAD_WORKLOAD_NUMBER = 20;
} // namespace


int work(const int& id)
{
    unique_lock<mutex> lock(global_mtx);
    cout << "Produced: " << id << endl;
    lock.unlock();
    int seconds = int((2.0 * rand()) / RAND_MAX + 5);
    this_thread::sleep_for(chrono::seconds(seconds));
    return id;
}

int main()
{    
    BlockingQueue<std::shared_future<int>> queue(MAX_THREAD_NUMBER);

    // Producer thread adds lots of worker threads to blocking queue
    std::thread producer_thread([&](){
        for (int i = 0; i < THREAD_WORKLOAD_NUMBER; i++)
        {
            shared_future<int> f = async(launch::async, work, i);
            queue.push(f);
        }

    });

    // Consumer thread (main thread) consumes the worker threads from blocking queue
    for (int i = 0; i < THREAD_WORKLOAD_NUMBER; i++)
    {
        shared_future<int> f = queue.front();
        int value = f.get();
        queue.pop();
        cout << "Consumed: " << value << endl;
    }
    return 0;
}