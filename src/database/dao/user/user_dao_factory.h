#pragma once

#include "mysql/user_mysql_dao.h"
#include "sqlite/user_sqlite_dao.h"

#include <memory>

// 角色表工厂类
class UserDaoFactory {
  public:
    UserDaoFactory()  = default;
    ~UserDaoFactory() = default;

    static std::shared_ptr<UserDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db) {
        if (mysql_db != nullptr) {
            return std::make_shared<UserMysqlDao>(mysql_db);
        } else {
            return std::make_shared<UserSqliteDao>(sqlite_db);
        }
    }
};