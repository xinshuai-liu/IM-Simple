#ifndef _MYSQLMANAGER_
#define _MYSQLMANAGER_

#include "../common/singleton.h"
#include "./mysqldao.h"

class MySQLManager : public Singleton<MySQLManager> {

public:
    ~MySQLManager();

    bool userLogin(const std::string& email, const std::string& passwd);
    bool userRegister(const std::string& name, const std::string& email, const std::string& passwd);
    bool userResetPasswd(const std::string& email, const std::string& passwd);

    bool CheckEmail(const std::string& name, const std::string& email);

private:
    friend class Singleton<MySQLManager>;
    MySQLManager();
    MySQLDAO  _dao;
};

#endif