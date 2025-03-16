#include "tcpsocket.h"


TcpSocket::TcpSocket(int fd) : _sockfd{ fd } {

}

TcpSocket::~TcpSocket() {
    if (_sockfd != -1) {
        close(_sockfd);
    }
}

void TcpSocket::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "fcntl failed: " << strerror(errno) << std::endl;
        throw std::runtime_error("Failed to get file descriptor flags");
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        throw std::runtime_error("Failed to set file descriptor to non-blocking");
    }
}


void TcpSocket::create() {
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1) {
        throw std::runtime_error("Failed to create socket");
    }
    //设置端口复用 
    int opt = 1;
    setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));
}

void TcpSocket::bind(const std::string& ip, int port) {

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    if (::bind(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
        throw std::runtime_error("Failed to bind socket");
    }

}

void TcpSocket::listen(int backlog) {
    if (::listen(_sockfd, backlog)) {
        throw std::runtime_error("Failed to listen on socket");
    }
}

int TcpSocket::accept(sockaddr_in* cliaddr) {

    sockaddr_in* cli_addr;
    socklen_t client_len;
    if (cliaddr == nullptr) {
        sockaddr_in addr;
        cli_addr = &addr;
    } else {
        cli_addr = cliaddr;
    }
    memset(cli_addr, 0, sizeof(&cli_addr));
    client_len = sizeof(&cli_addr);
    if (_sockfd == -1) {
        throw std::runtime_error("socket server fd value : -1");
    }
    int clifd = ::accept(_sockfd, (struct sockaddr*)cli_addr, &client_len);
    if (clifd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return clifd;
        } else {
            throw std::runtime_error("Failed to accept connection");
        }
    }
    return clifd;
}

void TcpSocket::send(const std::string& data, int fd) {
    if (fd == -1) fd = _sockfd;

    if (::send(fd, data.c_str(), data.size(), 0) == -1) {
        throw std::runtime_error("Failed to send data");
    }
}

std::string TcpSocket::recv(int fd) {
    if (fd == -1) fd = _sockfd;

    const int buffer_size = 1024;
    std::vector<char> buffer(buffer_size);
    std::string result = "";

    while (true) {
        ssize_t bytes_received = ::recv(fd, buffer.data(), buffer.size(), 0);
        if (bytes_received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // 非阻塞模式下，没有数据可读
                break;
            } else if (errno == ECONNRESET) {
                std::cerr << "Connection reset by peer, fd: " << fd << std::endl;
                break;
            } else {
                // 其他错误
                throw std::runtime_error("Failed to receive data: " + std::string(strerror(errno)));
            }
        } else if (bytes_received == 0) {
            // 对端关闭连接
            break;
        } else {
            // 成功读取数据
            result.append(buffer.data(), bytes_received);

            // 如果读取的数据小于缓冲区大小，说明没有更多数据可读
            if (static_cast<size_t>(bytes_received) < buffer.size()) {
                break;
            }
        }
    }

    return result;

    // const int buffer_size = 1024;
    // std::vector<char> buffer(buffer_size);
    // std::string result = "";

    // while (true) {
    //     ssize_t bytes_received = ::recv(_sockfd, buffer.data(), buffer.size(), 0);
    //     if (bytes_received == -1) {
    //         throw std::runtime_error("Failed to receive data");
    //     } else if (bytes_received == 0) {
    //         break;
    //     } else {
    //         result.append(buffer.data(), bytes_received);
    //         if (static_cast<long unsigned int>(bytes_received) < buffer.size()) {
    //             break;
    //         }
    //     }
    // }
    // return result;
}

void TcpSocket::close(int sockfd) {
    if (sockfd != -1) {
        ::close(sockfd);
        sockfd = -1;
    }
}

int TcpSocket::getSockfd() {
    return _sockfd;
}
