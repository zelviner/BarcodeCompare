#pragma once

#include "database/dao/mode/mode_dao.h"
#include "database/myorm/database.h"
#include "modes.hpp"

#include <memory>
#include <vector>

class ModeMysqlDao : public ModeDao {
  public:
    ModeMysqlDao(const std::shared_ptr<zel::myorm::Database> &db);
    ~ModeMysqlDao();

    /// @brief Add a new mode to the database.
    bool add(const std::shared_ptr<Mode> &mode);

    /// @brief Get all modes from the database.
    std::vector<std::shared_ptr<Mode>> all();

    /// @brief Get a mode by id and return the mode object
    std::shared_ptr<Mode> get(const int &id);

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<zel::myorm::Database> db_;
    std::shared_ptr<Modes>                modes_;
};