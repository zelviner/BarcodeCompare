#pragma once

#include "box_data_dao.h"
#include "sqlite/box_data_sqlite_dao.h"

#include <memory>

// 导入器工厂类
class BoxDataDaoFactory {
  public:
    enum DatabaseType { SQLITE, MYSQL };

  public:
    BoxDataDaoFactory() {}
    ~BoxDataDaoFactory() {}

    static std::shared_ptr<BoxDataDao> create(DatabaseType database_type, const std::shared_ptr<SQLite::Database> &sqlite_db, const std::string &order_name) {
        switch (database_type) {
        case SQLITE:
            return std::make_shared<BoxDataSqliteDao>(sqlite_db, order_name);

        case MYSQL:
            return std::make_shared<BoxDataSqliteDao>(sqlite_db, order_name);

        default:
            return std::make_shared<BoxDataSqliteDao>(sqlite_db, order_name);
        }
    }
};