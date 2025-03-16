#ifndef _NETWORKMANAGER_
#define _NETWORKMANAGER_

#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <unistd.h>
#include <arpa/inet.h>
#include <json/json.h>

#include "./httpmanager.h"
#include "./epoll.h"
#include "../common/singleton.h"


class NetworkManager : public Singleton<NetworkManager> {
public:
    ~NetworkManager();
    void run(const std::string& ip, int port);


    void sendData(const std::string& data);
    void recvDataHandler(std::string data, int fd);

private:
    friend  Singleton<NetworkManager>;
    std::map<std::string, std::function<std::string(const std::string&)>> _handlers;
    NetworkManager();
    void registerHandler();
    Epoll _epoll;
    bool _flag;
};

#endif

