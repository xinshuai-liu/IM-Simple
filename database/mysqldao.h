#ifndef _MYSQLDAO_
#define _MYSQLDAO_
#include "mysqlpool.h"

class MySQLDAO
{
public:
    MySQLDAO();
    ~MySQLDAO();

    bool userLogin(const std::string& email, const std::string& passwd);

    bool CheckEmail(const std::string& name, const std::string& email);

    bool userRegister(const std::string& name, const std::string& email, const std::string& passwd);
    bool userResetPasswd(const std::string& email, const std::string& passwd);


    // int RegUser(const std::string& name, const std::string& email, const std::string& pwd);
    // bool UpdatePwd(const std::string& name, const std::string& newpwd);
    // bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
    //int RegUserTransaction(const std::string& name, const std::string& email, const std::string& pwd, const std::string& icon);
    //bool CheckPwd(const std::string& name, const std::string& pwd, UserInfo& userInfo);
    //bool TestProcedure(const std::string& email, int& uid, string& name);
private:
    std::unique_ptr<MySQLPool> _pool;
};



#endif 