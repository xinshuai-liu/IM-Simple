#include "httpmanager.h"

HttpManager::HttpManager() {
    registerHandler();
}

HttpManager::~HttpManager() {
}

bool HttpManager::isHttp(const std::string& data) {
    // 检查数据是否为空
    if (data.empty()) {
        return false;
    }

    // 查找第一行的结束位置
    size_t first_line_end = data.find("\r\n");
    if (first_line_end == std::string::npos) {
        return false;
    }

    // 提取第一行
    std::string first_line = data.substr(0, first_line_end);

    // 检查是否是 HTTP 请求行
    if (first_line.find("HTTP/") != std::string::npos) {
        // 可能是 HTTP 响应
        return true;
    }

    // 检查是否是 HTTP 请求行
    size_t method_end = first_line.find(' ');
    if (method_end == std::string::npos) {
        return false;
    }

    std::string method = first_line.substr(0, method_end);
    std::vector<std::string> http_methods = { "GET", "POST", "PUT", "DELETE", "HEAD", "OPTIONS", "PATCH" };
    if (std::find(http_methods.begin(), http_methods.end(), method) != http_methods.end()) {
        // 可能是 HTTP 请求
        return true;
    }

    return false;
}

std::string  HttpManager::post(const std::string& data) {

    std::string result;

    // 解析 HTTP 请求
    std::istringstream request_stream(data);
    std::string method, path, version;
    request_stream >> method >> path >> version;

    Json::Value root;

    if (method.c_str() && path.c_str() && version.c_str() && method == "POST") {
        // 查找 Content-Length 头
        size_t content_length_pos = data.find("Content-Length: ");
        if (content_length_pos != std::string::npos) {
            size_t content_length_start = content_length_pos + 16; // "Content-Length: " 的长度
            size_t content_length_end = data.find("\r\n", content_length_start);
            std::string content_length_str = data.substr(content_length_start, content_length_end - content_length_start);
            int content_length = std::stoi(content_length_str);

            std::cout << "Content-Length: " << content_length << std::endl;

            size_t body_start_pos = data.find("\r\n\r\n");
            if (body_start_pos != std::string::npos) {
                body_start_pos += 4; // 跳过空行

                std::string body = data.substr(body_start_pos, content_length);

                Json::CharReaderBuilder reader;
                std::string errs;
                std::istringstream s(body);
                bool success = Json::parseFromStream(reader, s, &root, &errs);
                if (success) {
                    // std::cout << "Parsed JSON data:" << std::endl;

                    // for (const auto& key : root.getMemberNames()) {
                    //     std::cout << key << ": " << root[key] << std::endl;
                    // }
                } else {
                    std::cerr << "JSON parsing error: " << errs << std::endl;
                    return result;
                }
            }
        }
        result = _handlers[path](root);
    }

    return result;
}

void HttpManager::registerHandler() {
    _handlers.insert(std::map<std::string, std::function<std::string(Json::Value & root)>>::value_type("/user_login", [](Json::Value& root) -> std::string {
        std::string body;

        std::string email = root["email"].asCString();
        std::string passwd = root["passwd"].asCString();

        bool success = MySQLManager::getInstance().userLogin(email, passwd);
        if (success) {
            body = R"({"error":"success"})";
        } else {
            body = R"({"error":"error"})";
        }

        std::string response = "HTTP/1.1 200 OK\r\n"; // 状态行
        response += "Content-Type: application/json\r\n"; // 响应头
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n"; // 响应体长度
        response += "Connection: close\r\n"; // 关闭连接
        response += "\r\n"; // 空行，分隔头和体
        response += body; // 响应体

        std::cout << "/user_login" << body << std::endl;

        return response;
        }));
    _handlers.insert(std::map<std::string, std::function<std::string(Json::Value & root)>>::value_type("/get_verifycode", [](Json::Value& root) -> std::string {
        std::string body;

        std::string email = root["email"].asCString();
        // 创建RedisManager对象，传入密码
        RedisManager redisManager("0.0.0.0", 6379, "passwd");

        // 连接到Redis
        redisManager.connect();

        std::string verifyCode = std::to_string(::genRandom());

        // 设置键值对
        redisManager.set(email, verifyCode);

        // 设置过期时间
        redisManager.expire(email, 600);

        // 断开连接
        redisManager.disconnect();

        body = R"({"error":")" + verifyCode + R"("})";
        std::string response = "HTTP/1.1 200 OK\r\n"; // 状态行
        response += "Content-Type: application/json\r\n"; // 响应头
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n"; // 响应体长度
        response += "Connection: close\r\n"; // 关闭连接
        response += "\r\n"; // 空行，分隔头和体
        response += body; // 响应体

        std::cout << "/get_verifycode" << body << std::endl;

        return response;
        }));
    _handlers.insert(std::map<std::string, std::function<std::string(Json::Value & root)>>::value_type("/user_register", [](Json::Value& root) -> std::string {
        std::string body;
        std::string name = root["user"].asCString();
        std::string email = root["email"].asCString();
        std::string passwd = root["passwd"].asCString();
        std::string verifyCode = root["verifyCode"].asCString();

        RedisManager redisManager("0.0.0.0", 6379, "passwd");
        redisManager.connect();
        std::string value = redisManager.get(email);
        redisManager.disconnect();

        if (value == "" || value != verifyCode) {
            body = R"({"error":"verify code fail"})";
        } else {
            bool success = MySQLManager::getInstance().userRegister(name, email, passwd);
            if (success) {
                body = R"({"error":"success"})";
            } else {
                body = R"({"error":"register fail"})";
            }

        }
        std::string response = "HTTP/1.1 200 OK\r\n"; // 状态行
        response += "Content-Type: application/json\r\n"; // 响应头
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n"; // 响应体长度
        response += "Connection: close\r\n"; // 关闭连接
        response += "\r\n"; // 空行，分隔头和体
        response += body; // 响应体

        std::cout << "/user_register" << body << std::endl;

        return response;
        }));
    _handlers.insert(std::map<std::string, std::function<std::string(Json::Value & root)>>::value_type("/user_resetPasswd", [](Json::Value& root) -> std::string {
        std::string body;
        std::string name = root["user"].asCString();
        std::string email = root["email"].asCString();
        std::string passwd = root["passwd"].asCString();
        std::string verifyCode = root["verifyCode"].asCString();

        RedisManager redisManager("0.0.0.0", 6379, "passwd");
        redisManager.connect();
        std::string value = redisManager.get(email);
        redisManager.disconnect();

        if (value == "" || value != verifyCode) {
            body = R"({"error":"verify code fail"})";
        } else {
            bool success = MySQLManager::getInstance().userResetPasswd(email, passwd);
            if (success) {
                body = R"({"error":"success"})";
            } else {
                body = R"({"error":"reset password fail"})";
            }

        }
        std::string response = "HTTP/1.1 200 OK\r\n"; // 状态行
        response += "Content-Type: application/json\r\n"; // 响应头
        response += "Content-Length: " + std::to_string(body.size()) + "\r\n"; // 响应体长度
        response += "Connection: close\r\n"; // 关闭连接
        response += "\r\n"; // 空行，分隔头和体
        response += body; // 响应体

        std::cout << "/user_resetPasswd" << body << std::endl;

        return response;





        }));
    // _handlers.insert(std::map<std::string, std::function<std::string(Json::Value & root)>>::value_type("/get", [](Json::Value& root) -> std::string {

    //     }));
}

