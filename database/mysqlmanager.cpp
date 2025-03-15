#include "mysqlmanager.h"

MySQLManager::~MySQLManager() {
}

bool MySQLManager::userLogin(const std::string& email, const std::string& passwd) {
    return _dao.userLogin(email, passwd);
}

bool MySQLManager::userRegister(const std::string& name, const std::string& email, const std::string& passwd) {
    return _dao.userRegister(name, email, passwd);
}

bool MySQLManager::userResetPasswd(const std::string& email, const std::string& passwd) {
    return _dao.userResetPasswd(email, passwd);
}


bool MySQLManager::CheckEmail(const std::string& name, const std::string& email) {
    return _dao.CheckEmail(name, email);
}

MySQLManager::MySQLManager() {
}
