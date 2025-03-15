#ifndef _TCPSOCKET_
#define _TCPSOCKET_

#include <string>

namespace network {
    class TcpSocket;
}

class TcpSocket {
public:
    TcpSocket();
    ~TcpSocket();
    void create();
    void setNonBlocking();
    void bind(const std::string& ip, int port);
    void listen(int backlog);
    TcpSocket accept();
    void send(const std::string& data);
    std::string recv();
    void close(int sockfd);

private:
    int _sockfd;
};

#endif