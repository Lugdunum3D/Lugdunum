#pragma once

#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <lug/System/Export.hpp>

namespace lug {
namespace System {

class LUG_SYSTEM_API ThreadPool {
public:
    ThreadPool(uint8_t workerCount = 0);

    ThreadPool(const ThreadPool& rhs) = delete;
    ThreadPool(ThreadPool&& rhs) = delete;

    ThreadPool& operator=(const ThreadPool& rhs) = delete;
    ThreadPool& operator=(ThreadPool&& rhs) = delete;

    ~ThreadPool();

    template<typename Func, typename ...Args>
    auto enqueue(Func&& func, Args&&... args);

private:
    void workerLoop();

private:
    std::queue<std::function<void()> > _queue;
    std::vector<std::thread> _workers;

    bool _stop{false};
    std::condition_variable _condition;
    std::mutex _mutex;
};

#include <lug/System/ThreadPool.inl>

} // System
} // lug

