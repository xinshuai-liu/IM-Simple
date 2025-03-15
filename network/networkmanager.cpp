#include "./networkmanager.h"
#include "networkmanager.h"

int a = 0;

NetworkManager::NetworkManager() {
    _flag = true;
    registerHandler();
}

NetworkManager::~NetworkManager() {
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

std::string NetworkManager::recvData(const std::string& ip, int port) {

    std::string data;

    _serverSock.create();
    _serverSock.bind(ip, port);
    _serverSock.listen(4);

    while (_flag) {
        TcpSocket cli = _serverSock.accept();
        data = cli.recv();
        if (HttpManager::isHttp(data)) {
            cli.send(_handlers["http"](data));
        } else {
            _handlers["other"](data);
        }
    }

    return data;
}