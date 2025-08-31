#pragma once

#include "role.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class RoleDao {

  public:
    RoleDao(const std::shared_ptr<SQLite::Database> &db);
    ~RoleDao();

    /// @brief Add a new role to the database
    bool add(const std::shared_ptr<Role> &role);

    /// @brief Get all roles form the database
    std::vector<std::shared_ptr<Role>> all();

    /// @brief Get a role by id and return the role object
    std::shared_ptr<Role> get(const int id);

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
};