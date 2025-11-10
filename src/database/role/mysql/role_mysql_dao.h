#pragma once

#include "database/role/role_dao.h"

#include <memory>
#include <vector>
#include <zel/myorm.h>

class RoleMysqlDao : public RoleDao {

  public:
    RoleMysqlDao(const std::shared_ptr<zel::myorm::Database> &db);
    virtual ~RoleMysqlDao() = default;

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
    std::shared_ptr<zel::myorm::Database> db_;
};