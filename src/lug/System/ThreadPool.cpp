#include <lug/System/ThreadPool.hpp>

#include <iostream>

namespace lug {
namespace System {

ThreadPool::ThreadPool(uint8_t workerCount) {
    // If not provided, use maximum number of threads supported by hardware
    if (!workerCount) {
        workerCount = static_cast<uint8_t>(std::thread::hardware_concurrency());
    }

    for (uint8_t i = 0; i < workerCount; ++i) {
        std::thread workerThread(std::bind(&ThreadPool::workerLoop, this));
        _workers.push_back(std::move(workerThread));
    }
}

ThreadPool::~ThreadPool() {
    _stop = true;
    _condition.notify_all();

    // Wait all workers finish their tasks
    for (auto& worker: _workers) {
        worker.join();
    }
}

void ThreadPool::workerLoop() {
    std::function<void()> task;

    while (1) {
        // Add an enclosing scope to destroy the mutex before executing the task
        // to let another worker take another task from the queue
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock, [&]{return _stop || !_queue.empty();});

            // TODO: Should we wait all tasks are finished before exiting or just the tasks the workers are assigned to ?
            if (_stop && _queue.empty()) {
                return;
            }

            // Get next task and pop it from queue
            task = std::move(_queue.front());
            _queue.pop();
        }

        task();
    }
}

} // System
} // lug
