#pragma once

#include "mysql/order_mysql_dao.h"
#include "sqlite/order_sqlite_dao.h"

#include <memory>

// 订单表工厂类
class OrderDaoFactory {
  public:
    OrderDaoFactory()  = default;
    ~OrderDaoFactory() = default;

    static std::shared_ptr<OrderDao> create(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::shared_ptr<zel::myorm::Database> &mysql_db) {
        if (mysql_db != nullptr) {
            return std::make_shared<OrderMysqlDao>(mysql_db);
        } else {
            return std::make_shared<OrderSqliteDao>(sqlite_db);
        }
    }
};