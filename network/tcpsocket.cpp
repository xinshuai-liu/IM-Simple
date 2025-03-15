

#include "tcpsocket.h"
#include <stdexcept>

#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <string.h>

TcpSocket::TcpSocket() : _sockfd{ -1 } {}

TcpSocket::~TcpSocket() {
    if (_sockfd != -1) {
        close(_sockfd);
    }
}

void TcpSocket::setNonBlocking() {
    int flags = fcntl(_sockfd, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("Failed to get file descriptor flags");
    }
    if (fcntl(_sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
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

TcpSocket TcpSocket::accept() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sockfd = ::accept(_sockfd, (struct sockaddr*)&client_addr, &client_len);
    if (client_sockfd == -1) {
        throw std::runtime_error("Failed to accept connection");
    }

    TcpSocket client_socket;
    client_socket._sockfd = client_sockfd;
    return client_socket;
}

void TcpSocket::send(const std::string& data) {
    //if (data == "") return;
    if (::send(_sockfd, data.c_str(), data.size(), 0) == -1) {
        throw std::runtime_error("Failed to send data");
    }
}

std::string TcpSocket::recv() {
    const int buffer_size = 1024;
    std::vector<char> buffer(buffer_size);
    std::string result = "";

    while (true) {
        ssize_t bytes_received = ::recv(_sockfd, buffer.data(), buffer.size(), 0);
        if (bytes_received == -1) {
            throw std::runtime_error("Failed to receive data");
        } else if (bytes_received == 0) {
            break;
        } else {
            result.append(buffer.data(), bytes_received);
            if (bytes_received < buffer.size()) {
                break;
            }
        }
    }
    return result;
}

void TcpSocket::close(int sockfd) {
    if (sockfd != -1) {
        ::close(sockfd);
        sockfd = -1;
    }
}