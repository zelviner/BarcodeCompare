#pragma once

#include "database/dao/mode/mode_dao.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class ModeSqliteDao : public ModeDao {
  public:
    ModeSqliteDao(const std::shared_ptr<SQLite::Database> &db);
    virtual ~ModeSqliteDao() = default;

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
    std::shared_ptr<SQLite::Database> db_;
};