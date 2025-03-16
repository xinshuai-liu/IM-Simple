#include "./networkmanager.h"
#include "networkmanager.h"

NetworkManager::NetworkManager() {
    _flag = true;
    registerHandler();
}

NetworkManager::~NetworkManager() {
}

void NetworkManager::run(const std::string& ip, int port) {
    try {
        _epoll.monitor(ip, port);
        _epoll.setRecvEventHandler([this](std::string data, int fd) {recvDataHandler(data, fd);});
        _epoll.run(_flag);
    }
    catch (std::exception& exp) {
        std::cout << "recvDate: " << exp.what() << std::endl;
    }

}

void NetworkManager::registerHandler() {

    _handlers.insert(std::map<std::string, std::function<std::string(const std::string&)>>::value_type("http", [](const std::string& data) -> std::string {
        return HttpManager::getInstance().post(data);
        }));

    _handlers.insert(std::map<std::string, std::function<std::string(const std::string&)>>::value_type("other", [](const std::string& data) -> std::string {
        std::cout << "other: " << data << std::endl;
        return "";
        }));
}

void NetworkManager::recvDataHandler(std::string data, int fd) {
    std::cout << data << std::endl;

    if (HttpManager::isHttp(data)) {
        _epoll.send(_handlers["http"](data), fd);
    } else {
        _handlers["other"](data);
    }

}

