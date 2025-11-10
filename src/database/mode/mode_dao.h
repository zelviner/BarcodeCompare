#pragma once

#include "mode.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class ModeDao {
  public:
    /// @brief Add a new mode to the database.
    virtual bool add(const std::shared_ptr<Mode> &mode) = 0;

    /// @brief Get all modes from the database.
    virtual std::vector<std::shared_ptr<Mode>> all() = 0;

    /// @brief Get a mode by id and return the mode object
    virtual std::shared_ptr<Mode> get(const int &id) = 0;
};