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

    // 提交任务到线程池
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    // 获取线程池中线程的数量
    size_t getThreadCount() const;

private:
    // 工作线程
    std::vector<std::thread> workers;

    // 任务队列
    std::queue<std::function<void()>> tasks;

    // 同步
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;
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