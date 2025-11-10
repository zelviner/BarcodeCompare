#pragma once

#include "mysql/card_data_mysql_dao.h"
#include "sqlite/card_data_sqlite_dao.h"

#include <memory>

// 外箱数据库工厂类
class CardDataDaoFactory {
  public:
    CardDataDaoFactory()  = default;
    ~CardDataDaoFactory() = default;

    static std::shared_ptr<CardDataDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db,
                                               const std::string &order_name) {
        if (mysql_db != nullptr) {
            return std::make_shared<CardDataMysqlDao>(order_name);
        } else {
            return std::make_shared<CardDataSqliteDao>(sqlite_db, order_name);
        }
    }
};