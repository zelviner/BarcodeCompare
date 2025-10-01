#pragma once

#include "mysql/format_mysql_dao.h"
#include "sqlite/format_sqlite_dao.h"

#include <memory>

// 文件格式工厂类
class FormatDaoFactory {
  public:
    FormatDaoFactory()  = default;
    ~FormatDaoFactory() = default;

    static std::shared_ptr<FormatDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db) {
        if (mysql_db != nullptr) {
            return std::make_shared<FormatMysqlDao>(mysql_db);
        } else {
            return std::make_shared<FormatSqliteDao>(sqlite_db);
        }
    }
};