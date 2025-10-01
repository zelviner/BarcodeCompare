#pragma once

#include "role.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class RoleDao {
  public:
    /// @brief Add a new role to the database
    virtual bool add(const std::shared_ptr<Role> &role) = 0;

    /// @brief Get all roles form the database
    virtual std::vector<std::shared_ptr<Role>> all() = 0;

    /// @brief Get a role by id and return the role object
    virtual std::shared_ptr<Role> get(const int id) = 0;
};