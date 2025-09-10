#pragma once

#include "data/format.h"
#include "order.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class OrderDao {

  public:
    OrderDao(const std::shared_ptr<SQLite::Database> &db);
    ~OrderDao();

    /// @brief Add a new order to the database.
    bool add(const std::shared_ptr<Order> &order);

    /// @brief Remove an order from the database by its id.
    bool remove(const int &id);

    /// @brief Clear all orders from the database.
    bool clear();

    /// @brief Update an order in the database by its id.
    bool update(const int &id, const std::shared_ptr<Order> &order);

    /// @brief Get all orders from the database.
    std::vector<std::shared_ptr<Order>> all();

    /// @brief Get an order by its id and name.
    std::shared_ptr<Order> get(const int &id);
    std::shared_ptr<Order> get(const std::string &name);

    /// @brief Check if an order with the given name exists in the database.
    bool exists(const std::string &name);

    /// @brief Set the current order.
    bool currentOrder(const int &order_id);

    /// @brief Get the current order.
    std::shared_ptr<Order> currentOrder();

  private:
    /// @brief Initialize the data and create tables.
    void init();

    bool hasRequiredValues(const std::vector<std::string> &headers, const std::shared_ptr<Format> &format);

  private:
    std::shared_ptr<SQLite::Database> db_;
    int                               current_order_id_;
};