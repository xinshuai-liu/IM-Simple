#include "epoll.h"
#include <sys/socket.h>
#include <arpa/inet.h>


Epoll::Epoll() :_instance{ -1 } {
    _instance = epoll_create(1);
    if (_instance == -1) {
        throw std::runtime_error("Failed to create epoll instance: " + std::string(strerror(errno)));
    }
    _data = new std::string;
    _recvEventHandler = [](std::string, int fd) {std::cout << "epoll init recv event handle" << std::endl;};
    _sendEventHandler = [](std::string) {};
}

Epoll::~Epoll() {
    if (_instance != -1) {
        close(_instance);
    }
    delete _data;
}

void Epoll::monitor(const std::string& ip, int port) {
    this->create();
    this->setNonBlocking(_sockfd);
    this->bind(ip, port);
    this->listen(128);

    add(_sockfd, EPOLLIN | EPOLLET);
}


void Epoll::add(int fd, uint32_t event) {

    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;

    if (epoll_ctl(_instance, EPOLL_CTL_ADD, fd, &ev) == -1) {
        close(fd);
        throw std::runtime_error("Failed to add fd to epoll: " + std::string(strerror(errno)));
    }
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

    close(fd); // 关闭文件描述符
}
/*
// void Epoll::run(ThreadPool& pool, bool flag = true, int timeout = -1) {

//     struct epoll_event events[MAX_EVENTS];

//     int numEvents = epoll_wait(_instance, events, MAX_EVENTS, timeout);
//     if (numEvents == -1) {
//         throw std::runtime_error("Failed to wait for epoll events: " + std::string(strerror(errno)));
//     }

//     for (int i = 0; i < numEvents; ++i) {

//         // if (_callbacks.find(events[i].data.fd) != _callbacks.end()) {
//         //     //pool.enqueue([this, events, i]() {_callbacks[events[i].data.fd](events[i].data.fd, events[i].events);});
//         // }
//     }
// }
*/

void Epoll::run(bool flag, int timeout) {
    ThreadPool pool(4);
    struct epoll_event events[MAX_EVENTS];
    while (flag) {
        int numEvents = epoll_wait(_instance, events, MAX_EVENTS, timeout);
        if (numEvents == -1) {
            throw std::runtime_error("Failed to wait for epoll events: " + std::string(strerror(errno)));
        }
        for (int i = 0; i < numEvents; ++i) {
            uint32_t event = events[i].events;
            int fd = events[i].data.fd;
            if (fd == _sockfd) {
                pool.enqueue([this, event]() {
                    std::cout << std::this_thread::get_id() << std::endl;
                    managerHandler(event);
                    });
            } else {
                pool.enqueue([this, fd, event]() {
                    std::cout << std::this_thread::get_id() << std::endl;
                    workHandler(fd, event);
                    });
            }
        }
    }
}

void Epoll::managerHandler(uint32_t event) {
    if (event & EPOLLRDHUP) {
        std::cout << "closed connection, fd = " << _sockfd << std::endl;
        remove(_sockfd);
    }
    if (event & EPOLLIN) {
        while (true) {
            int clifd;
            try {
                sockaddr_in addr;
                clifd = this->accept(&addr);
                if (clifd == -1) {
                    break;
                }
                TcpSocket::setNonBlocking(clifd);

                std::cout << "client connection from: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port)
                    << ", clientfd = " << clifd << std::endl;

                add(clifd, EPOLLIN | EPOLLET);
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Failed to add client fd to epoll: " << e.what() << std::endl;
                if (clifd != -1) {
                    close(clifd);
                }
                return;
            }
        }
    }
}

void Epoll::workHandler(int fd, uint32_t event) {
    if (event & EPOLLRDHUP) {
        std::cout << "Client closed connection, fd = " << fd << std::endl;
        remove(fd);
    }
    if (event & EPOLLIN) {
        std::string data;
        try {
            data = recv(fd);
            if (data.size() == 0) {
                std::cout << "Client closed connection, fd = " << fd << std::endl;
                remove(fd);
            }
            _recvEventHandler(data, fd);
        }
        catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}



