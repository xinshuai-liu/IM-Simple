#ifndef _REDISMANAGER_
#define _REDISMANAGER_

#include <hiredis/hiredis.h>
#include <string>
#include <stdexcept>

class RedisManager {
public:
    // 构造函数和析构函数
    RedisManager(const std::string& host = "0.0.0.0", int port = 6379, const std::string& password = "");
    ~RedisManager();

    // 连接Redis服务器
    void connect();

    // 断开连接
    void disconnect();

    // 设置键值对
    void set(const std::string& key, const std::string& value);

    // 获取键值
    std::string get(const std::string& key);

    // 删除键
    void del(const std::string& key);

    // 检查键是否存在
    bool exists(const std::string& key);

    // 设置键的过期时间（秒）
    void expire(const std::string& key, int seconds);

    // 获取键的剩余过期时间
    long long ttl(const std::string& key);

private:
    std::string _host;       // Redis服务器地址
    int _port;               // Redis服务器端口
    std::string _password;   // Redis认证密码
    redisContext* _context;  // Redis连接上下文

    // 检查连接是否有效
    void checkConnection() const;

    // 认证
    void authenticate();
};




#endif