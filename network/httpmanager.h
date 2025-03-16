#ifndef _HTTPMANAGER_
#define _HTTPMANAGER_

#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <arpa/inet.h>
#include <json/json.h>
#include <functional>

#include "../common/global.h"
#include "../common/singleton.h"
// #include "./tcpsocket.h"

namespace network {
    class HttpManager;
}

class HttpManager : public Singleton<HttpManager>, public std::enable_shared_from_this<HttpManager>
{
public:
    ~HttpManager();

    static bool isHttp(const std::string& data);
    std::string  post(const std::string& data);
private:
    void registerHandler();
    std::map<std::string, std::function<std::string(Json::Value& root)>> _handlers;
    friend Singleton<HttpManager>;
    HttpManager();
};

#endif