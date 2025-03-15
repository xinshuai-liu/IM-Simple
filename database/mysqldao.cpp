#include "./mysqldao.h"
#include "mysqldao.h"

MySQLDAO::MySQLDAO() {
    _pool.reset(new MySQLPool("0.0.0.0:3306", "root", "passwd", "im", 5));

}

MySQLDAO::~MySQLDAO() {
    _pool->Close();
}

bool MySQLDAO::userLogin(const std::string& email, const std::string& passwd) {
    auto con = _pool->getConnection();
    try {
        if (con == nullptr) {
            _pool->returnConnection(std::move(con));
            return false;
        }
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("select pwd from user where email = ?"));
        pstmt->setString(1, email);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());
        while (res->next()) {
            std::cout << "Check Passwd: " << res->getString("pwd") << std::endl;
            if (passwd != res->getString("pwd")) {
                _pool->returnConnection(std::move(con));
                return false;
            }
            _pool->returnConnection(std::move(con));
            return true;
        }
    }
    catch (sql::SQLException& e) {
        _pool->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return false;
}
bool MySQLDAO::CheckEmail(const std::string& name, const std::string& email) {
    auto con = _pool->getConnection();
    try {
        if (con == nullptr) {
            _pool->returnConnection(std::move(con));
            return false;
        }

        // 准备查询语句
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("SELECT email FROM user WHERE name = ?"));

        // 绑定参数
        pstmt->setString(1, name);

        // 执行查询
        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        // 遍历结果集
        while (res->next()) {
            std::cout << "Check Email: " << res->getString("email") << std::endl;
            if (email != res->getString("email")) {
                _pool->returnConnection(std::move(con));
                return false;
            }
            _pool->returnConnection(std::move(con));
            return true;
        }
    }
    catch (sql::SQLException& e) {
        _pool->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return false;
}

bool MySQLDAO::userRegister(const std::string& name, const std::string& email, const std::string& passwd) {
    auto con = _pool->getConnection();
    try {
        if (con == nullptr) {
            _pool->returnConnection(std::move(con));
            return false;
        }
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("insert into user(name, email, pwd) values(?,?,?)"));
        pstmt->setString(1, name);
        pstmt->setString(2, email);
        pstmt->setString(3, passwd);

        bool success = pstmt->executeUpdate();
        return success;
    }
    catch (sql::SQLException& e) {
        _pool->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return false;
}

bool MySQLDAO::userResetPasswd(const std::string& email, const std::string& passwd) {
    auto con = _pool->getConnection();
    try {
        if (con == nullptr) {
            _pool->returnConnection(std::move(con));
            return false;
        }
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("update user set pwd = ? where email = ?"));
        pstmt->setString(1, passwd);
        pstmt->setString(2, email);

        bool success = pstmt->executeUpdate();
        return success;
    }
    catch (sql::SQLException& e) {
        _pool->returnConnection(std::move(con));
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }
    return false;

}
