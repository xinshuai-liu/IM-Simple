#include "epoll.h"
#include <string.h>

Epoll::Epoll() :_instance{ -1 } {
    _instance = epoll_create(0);
    if (_instance == -1) {
        throw std::runtime_error("Failed to create epoll instance: " + std::string(strerror(errno)));
    }
}

Epoll::~Epoll() {
    if (_instance != -1) {
        close(_instance);
    }
}

void Epoll::add(int fd, uint32_t event, const Callback& callback) {

    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    if (epoll_ctl(_instance, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("Failed to add fd to epoll: " + std::string(strerror(errno)));
    }

    _callbacks[fd] = callback; // 保存回调函数
}

void Epoll::modify(int fd, uint32_t event) {

    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    if (epoll_ctl(_instance, EPOLL_CTL_MOD, fd, &ev) == -1) {
        throw std::runtime_error("Failed to modify fd in epoll: " + std::string(strerror(errno)));
    }
}

void Epoll::remove(int fd) {
    if (epoll_ctl(_instance, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        throw std::runtime_error("Failed to remove fd from epoll: " + std::string(strerror(errno)));
    }

    _callbacks.erase(fd); // 移除回调函数
    close(fd); // 关闭文件描述符
}

void Epoll::run(int timeout) {

    struct epoll_event events[MAX_EVENTS];

    int numEvents = epoll_wait(_instance, events, MAX_EVENTS, timeout);
    if (numEvents == -1) {
        throw std::runtime_error("Failed to wait for epoll events: " + std::string(strerror(errno)));
    }

    for (int i = 0; i < numEvents; ++i) {

        if (_callbacks.find(events[i].data.fd) != _callbacks.end()) {
            _callbacks[events[i].data.fd](events[i].data.fd, events[i].events);
        }
    }
}
