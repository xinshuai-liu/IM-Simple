// mysqlManager
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "../common/singleton.h"

class MySQLManager : public Singleton<MySQLManager> {
    friend class Singleton<MySQLManager>; // 允许 Singleton 访问私有构造函数

public:
    // 获取一个数据库连接
    std::shared_ptr<sql::Connection> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (connections_.empty()) {
            cond_.wait(lock); // 等待直到有可用连接
        }
        auto conn = connections_.back();
        connections_.pop_back();
        return conn;
    }

    // 释放一个数据库连接
    void releaseConnection(std::shared_ptr<sql::Connection> conn) {
        std::unique_lock<std::mutex> lock(mutex_);
        connections_.push_back(conn);
        cond_.notify_one(); // 通知等待的线程
    }

    // 初始化连接池
    void initialize(const std::string& host, const std::string& user, const std::string& password, const std::string& database, int poolSize = 5) {
        std::lock_guard<std::mutex> lock(mutex_);
        driver_ = sql::mysql::get_mysql_driver_instance();
        for (int i = 0; i < poolSize; ++i) {
            auto conn = std::shared_ptr<sql::Connection>(
                driver_->connect(host, user, password)
            );
            conn->setSchema(database);
            connections_.push_back(conn);
        }
    }

private:
    MySQLManager() = default; // 私有构造函数
    ~MySQLManager() {
        for (auto& conn : connections_) {
            conn->close();
        }
    }

    sql::Driver* driver_; // MySQL 驱动
    std::vector<std::shared_ptr<sql::Connection>> connections_; // 连接池
    std::mutex mutex_; // 互斥锁
    std::condition_variable cond_; // 条件变量
};