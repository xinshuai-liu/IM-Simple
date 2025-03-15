#ifndef _NETWORKMANAGER_
#define _NETWORKMANAGER_

#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <arpa/inet.h>
#include <json/json.h>

#include "./tcpsocket.h"
#include "./httpmanager.h"
#include "../common/singleton.h"

class NetworkManager : public Singleton<NetworkManager> {
public:
    ~NetworkManager();
    void sendData(const std::string& data);
    std::string recvData(const std::string& ip, int port);

private:
    friend  Singleton<NetworkManager>;
    NetworkManager();
    void registerHandler();
    std::map<std::string, std::function<std::string(const std::string&)>> _handlers;
    TcpSocket _serverSock;
    bool _flag;
};

#endif

