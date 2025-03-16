#include "threadpool.h"

// 构造函数，启动工作线程
ThreadPool::ThreadPool(size_t numThreads) : _stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        _workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(_queueMutex);
                    _condition.wait(lock, [this] { return _stop || !_tasks.empty(); });
                    if (_stop && _tasks.empty()) return;
                    task = std::move(_tasks.front());
                    _tasks.pop();
                }
                task();
            }
            });
    }
}

// 析构函数，停止所有线程
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _stop = true;
    }
    _condition.notify_all();
    for (std::thread& worker : _workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        if (_stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        _tasks.emplace(task);
    }
    _condition.notify_one();
}

// 获取线程池中线程的数量
size_t ThreadPool::getThreadCount() const {
    return _workers.size();
}