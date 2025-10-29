#pragma once

#include "database/dao/format/format_dao.h"
#include "database/myorm/database.h"

#include <memory>
#include <vector>

class FormatMysqlDao : public FormatDao {

  public:
    FormatMysqlDao(const std::shared_ptr<zel::myorm::Database> &db);
    virtual ~FormatMysqlDao() = default;

    /// @brief Add a new role to the database
    bool add(const std::shared_ptr<Format> &role) override;

    /// @brief Get all roles form the database
    std::vector<std::shared_ptr<Format>> all() override;

    /// @brief Get a role by id and return the role object
    std::shared_ptr<Format> get(const int id) override;

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<zel::myorm::Database> db_;
};