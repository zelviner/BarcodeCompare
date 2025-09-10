#pragma once

#include "format.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class FormatDao {

  public:
    FormatDao(const std::shared_ptr<SQLite::Database> &db);
    ~FormatDao();

    /// @brief Add a new role to the database
    bool add(const std::shared_ptr<Format> &role);

    /// @brief Get all roles form the database
    std::vector<std::shared_ptr<Format>> all();

    /// @brief Get a role by id and return the role object
    std::shared_ptr<Format> get(const int id);

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
};