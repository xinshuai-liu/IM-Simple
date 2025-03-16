#ifndef _TCPSOCKET_
#define _TCPSOCKET_

#include <string>
#include <stdexcept>

#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <string.h>

namespace network {
    class TcpSocket;
}

class TcpSocket {
public:
    TcpSocket(int fd = -1);
    ~TcpSocket();
    void create();
    static void setNonBlocking(int fd = -1);
    void bind(const std::string& ip, int port);
    void listen(int backlog);
    int accept(sockaddr_in* cliaddr = nullptr);
    void send(const std::string& data, int fd = -1);
    std::string recv(int fd = -1);
    void close(int sockfd);
    int getSockfd();
public:
    //struct sockaddr_in _cliaddr;
protected:
    int _sockfd;
};

#endif