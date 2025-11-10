#pragma once

#include "format.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class FormatDao {
  public:
    /// @brief Add a new role to the database
    virtual bool add(const std::shared_ptr<Format> &role) = 0;

    /// @brief Get all roles form the database
    virtual std::vector<std::shared_ptr<Format>> all() = 0;

    /// @brief Get a role by id and return the role object
    virtual std::shared_ptr<Format> get(const int id) = 0;
};