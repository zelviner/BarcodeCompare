#pragma once

#include "mysql/mode_mysql_dao.h"
#include "sqlite/mode_sqlite_dao.h"

#include <memory>

// 模式表工厂类
class ModeDaoFactory {
  public:
    ModeDaoFactory()  = default;
    ~ModeDaoFactory() = default;

    static std::shared_ptr<ModeDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db) {
        if (mysql_db != nullptr) {
            return std::make_shared<ModeMysqlDao>(mysql_db);
        } else {
            return std::make_shared<ModeSqliteDao>(sqlite_db);
        }
    }
};