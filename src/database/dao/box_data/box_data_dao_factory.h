#pragma once

#include "mysql/box_data_mysql_dao.h"
#include "sqlite/box_data_sqlite_dao.h"

#include <memory>

// 内盒数据库工厂类
class BoxDataDaoFactory {
  public:
    BoxDataDaoFactory()  = default;
    ~BoxDataDaoFactory() = default;

    static std::shared_ptr<BoxDataDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db,
                                              const std::string &order_name) {

        if (mysql_db != nullptr) {
            return std::make_shared<BoxDataMysqlDao>(mysql_db, order_name);
        } else {
            return std::make_shared<BoxDataSqliteDao>(sqlite_db, order_name);
        }
    }
};