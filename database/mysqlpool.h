#ifndef _MYSQLPOOL_
#define _MYSQLPOOL_

#include <string>
#include <stdexcept>
#include <thread>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <mutex>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>


class MySQLPool {
public:
    MySQLPool(const std::string& host, const std::string& user, const std::string& pass, const std::string& schema, int poolSize);
    ~MySQLPool();

    std::unique_ptr<sql::Connection> getConnection();
    void returnConnection(std::unique_ptr<sql::Connection> con);
    void Close();

private:
    std::string _host;
    std::string _user;
    std::string _passwd;
    std::string _schema;
    int _poolSize;
    std::queue<std::unique_ptr<sql::Connection>> _pool;
    std::mutex _mutex;
    std::condition_variable _cond;
    std::atomic<bool> _stop;
};

#endif