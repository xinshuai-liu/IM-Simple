#include <iostream>
#include <vector>
#include <functional>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <memory>
#include <string.h>

class Epoll {
public:

    using Callback = std::function<void(int, uint32_t)>;

    Epoll();

    ~Epoll();

    void add(int fd, uint32_t event, const Callback& callback);

    void modify(int fd, uint32_t event);

    void remove(int fd);

    void run(int timeout = -1);

    // 禁用拷贝构造函数和拷贝赋值运算符
    Epoll(const Epoll&) = delete;
    Epoll& operator=(const Epoll&) = delete;

    // 允许移动构造函数
    Epoll(Epoll&& other) noexcept : _instance{ other._instance } {
        other._instance = -1; // 将原对象置为无效状态
    }

    // 允许移动赋值运算符
    Epoll& operator=(Epoll&& other) noexcept {
        if (this != &other) {
            if (_instance != -1) {
                close(_instance); // 释放当前对象的资源
            }
            _instance = other._instance;
            other._instance = -1; // 将原对象置为无效状态
        }
        return *this;
    }

    // 获取 epoll 实例的文件描述符
    int get_instance() const {
        return _instance;
    }


private:
    static const int MAX_EVENTS = 64; // 每次处理的最大事件数
    int _instance; // epoll 文件描述符
    std::unordered_map<int, Callback> _callbacks; // 文件描述符到回调函数的映射
};
