#pragma once

#include "order.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class OrderDao {
  public:
    /// @brief Add a new order to the database.
    virtual bool add(const std::shared_ptr<Order> &order) = 0;

    /// @brief Remove an order from the database by its id.
    virtual bool remove(const int &id) = 0;

    /// @brief Clear all orders from the database.
    virtual bool clear() = 0;

    /// @brief Update an order in the database by its id.
    virtual bool update(const int &id, const std::shared_ptr<Order> &order) = 0;

    /// @brief Get all orders from the database.
    virtual std::vector<std::shared_ptr<Order>> all() = 0;

    /// @brief Get an order by its id or name.
    virtual std::shared_ptr<Order> get(const int &id)           = 0;
    virtual std::shared_ptr<Order> get(const std::string &name) = 0;

    /// @brief Check if an order with the given name exists in the database.
    virtual bool exists(const std::string &name) = 0;

    /// @brief Set the current order.
    virtual bool currentOrder(const int &order_id) = 0;

    /// @brief Get the current order.
    virtual std::shared_ptr<Order> currentOrder() = 0;
};