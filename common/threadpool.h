#ifndef _THREADPOOL_
#define _THREADPOOL_
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);

    size_t getThreadCount() const;
private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _queueMutex;
    std::condition_variable _condition;
    std::atomic<bool> _stop;
};




// // 示例使用
// int main() {
//     ThreadPool pool(4);

//     auto result1 = pool.enqueue([](int a, int b) { return a + b; }, 1, 2);
//     auto result2 = pool.enqueue([](int a, int b) { return a * b; }, 3, 4);

//     std::cout << "Result 1: " << result1.get() << std::endl;
//     std::cout << "Result 2: " << result2.get() << std::endl;

//     return 0;
// }
#endif