#pragma once

#include "mysql/role_mysql_dao.h"
#include "sqlite/role_sqlite_dao.h"

#include <memory>

// 角色表工厂类
class RoleDaoFactory {
  public:
    RoleDaoFactory()  = default;
    ~RoleDaoFactory() = default;

    static std::shared_ptr<RoleDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db) {
        if (mysql_db != nullptr) {
            return std::make_shared<RoleMysqlDao>(mysql_db);
        } else {
            return std::make_shared<RoleSqliteDao>(sqlite_db);
        }
    }
};