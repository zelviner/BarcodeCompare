#pragma once

#include "database/dao/role/role_dao.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class RoleSqliteDao : public RoleDao {

  public:
    RoleSqliteDao(const std::shared_ptr<SQLite::Database> &db);
    virtual ~RoleSqliteDao() = default;

    /// @brief Add a new role to the database
    bool add(const std::shared_ptr<Role> &role) override;

    /// @brief Get all roles form the database
    std::vector<std::shared_ptr<Role>> all() override;

    /// @brief Get a role by id and return the role object
    std::shared_ptr<Role> get(const int id) override;

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
};