#include "redismanager.h"
#include <iostream>

// 构造函数
RedisManager::RedisManager(const std::string& host, int port, const std::string& password)
    : _host(host), _port(port), _password(password), _context(nullptr) {
}

// 析构函数
RedisManager::~RedisManager() {
    disconnect();
}

// 连接Redis服务器
void RedisManager::connect() {
    _context = redisConnect(_host.c_str(), _port);
    if (_context == nullptr || _context->err) {
        if (_context) {
            std::cerr << "Connection error: " << _context->errstr << std::endl;
            redisFree(_context);
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        throw std::runtime_error("Failed to connect to Redis");
    }

    // 如果设置了密码，进行认证
    if (!_password.empty()) {
        authenticate();
    }

    std::cout << "Connected to Redis server at " << _host << ":" << _port << std::endl;
}

// 断开连接
void RedisManager::disconnect() {
    if (_context) {
        redisFree(_context);
        _context = nullptr;
        std::cout << "Disconnected from Redis server" << std::endl;
    }
}

// 检查连接是否有效
void RedisManager::checkConnection() const {
    if (!_context || _context->err) {
        throw std::runtime_error("Not connected to Redis");
    }
}

// 认证
void RedisManager::authenticate() {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "AUTH %s", _password.c_str());
    if (!reply || reply->type == REDIS_REPLY_ERROR) {
        if (reply) {
            std::cerr << "Authentication failed: " << reply->str << std::endl;
            freeReplyObject(reply);
        }
        throw std::runtime_error("Failed to authenticate with Redis");
    }
    freeReplyObject(reply);
    std::cout << "Authenticated successfully" << std::endl;
}

// 设置键值对
void RedisManager::set(const std::string& key, const std::string& value) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "SET %s %s", key.c_str(), value.c_str());
    if (!reply) {
        throw std::runtime_error("Failed to execute SET command");
    }
    freeReplyObject(reply);
}

// 获取键值
std::string RedisManager::get(const std::string& key) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "GET %s", key.c_str());
    if (!reply || reply->type == REDIS_REPLY_NIL) {
        freeReplyObject(reply);
        return "";
        //throw std::runtime_error("Key not found or GET command failed");
    }
    std::string value = reply->str;
    freeReplyObject(reply);
    return value;
}

// 删除键
void RedisManager::del(const std::string& key) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "DEL %s", key.c_str());
    if (!reply) {
        throw std::runtime_error("Failed to execute DEL command");
    }
    freeReplyObject(reply);
}

// 检查键是否存在
bool RedisManager::exists(const std::string& key) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "EXISTS %s", key.c_str());
    if (!reply) {
        throw std::runtime_error("Failed to execute EXISTS command");
    }
    bool exists = reply->integer == 1;
    freeReplyObject(reply);
    return exists;
}

// 设置键的过期时间
void RedisManager::expire(const std::string& key, int seconds) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "EXPIRE %s %d", key.c_str(), seconds);
    if (!reply) {
        throw std::runtime_error("Failed to execute EXPIRE command");
    }
    freeReplyObject(reply);
}

// 获取键的剩余过期时间
long long RedisManager::ttl(const std::string& key) {
    checkConnection();
    redisReply* reply = (redisReply*)redisCommand(_context, "TTL %s", key.c_str());
    if (!reply) {
        throw std::runtime_error("Failed to execute TTL command");
    }
    long long ttl = reply->integer;
    freeReplyObject(reply);
    return ttl;
}