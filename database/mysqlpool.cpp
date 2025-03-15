#include "mysqlpool.h"

MySQLPool::MySQLPool(const std::string& host, const std::string& user, const std::string& pass, const std::string& schema, int poolSize)
    : _host(host), _user(user), _passwd(pass), _schema(schema), _poolSize(poolSize), _stop(false) {
    try {
        for (int i = 0; i < _poolSize; ++i) {
            sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
            std::unique_ptr<sql::Connection> con(driver->connect(_host, _user, _passwd));
            con->setSchema(_schema);
            _pool.push(std::move(con));
        }
    }
    catch (sql::SQLException& e) {
        // 处理异常
        std::cout << "mysql pool init failed " << e.what() << std::endl;
    }
}

MySQLPool::~MySQLPool() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_pool.empty()) {
        _pool.pop();
    }
}

void MySQLPool::Close() {
    _stop = true;
    _cond.notify_all();
}

void MySQLPool::returnConnection(std::unique_ptr<sql::Connection> con) {
    std::unique_lock<std::mutex> lock(_mutex);
    if (_stop) {
        return;
    }
    _pool.push(std::move(con));
    _cond.notify_one();
}

std::unique_ptr<sql::Connection> MySQLPool::getConnection() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this] {
        if (_stop) {
            return true;
        }
        return !_pool.empty();
        });

    if (_stop) {
        return nullptr;
    }
    std::unique_ptr<sql::Connection> con(std::move(_pool.front()));
    _pool.pop();
    return con;
}