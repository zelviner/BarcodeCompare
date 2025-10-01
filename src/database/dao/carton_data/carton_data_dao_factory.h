#pragma once

#include "mysql/carton_data_mysql_dao.h"
#include "sqlite/carton_data_sqlite_dao.h"

#include <memory>

// 外箱数据库工厂类
class CartonDataDaoFactory {
  public:
    CartonDataDaoFactory()  = default;
    ~CartonDataDaoFactory() = default;

    static std::shared_ptr<CartonDataDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db,
                                                 const std::string &order_name) {
        if (mysql_db != nullptr) {
            return std::make_shared<CartonDataMysqlDao>(mysql_db, order_name);
        } else {
            return std::make_shared<CartonDataSqliteDao>(sqlite_db, order_name);
        }
    }
};