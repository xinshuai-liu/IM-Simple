#include <iostream>
#include <functional>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
#include <memory>
#include <string.h>
#include <unordered_map>

#include "../common/threadpool.h"
#include "tcpsocket.h"

class Epoll : public TcpSocket {
public:
    Epoll();
    ~Epoll();

    void monitor(const std::string& ip, int port);

    void add(int fd, uint32_t event);
    void modify(int fd, uint32_t event);
    void remove(int fd);

    void run(bool flag = true, int timeout = -1);

    void managerHandler(uint32_t event);
    void workHandler(int fd, uint32_t event);

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

    void setRecvEventHandler(std::function<void(std::string, int fd)> func) {
        _recvEventHandler = func;
    }
private:
    static const int MAX_EVENTS = 128; // 每次处理的最大事件数
    int _instance; // epoll 文件描述符
    std::string* _data;
    std::function<void(std::string, int fd)> _recvEventHandler;
    std::function<void(std::string)> _sendEventHandler;
};
