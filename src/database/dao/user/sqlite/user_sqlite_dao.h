#pragma once

#include "database/dao/user/user_dao.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class UserSqliteDao : public UserDao {

  public:
    UserSqliteDao(const std::shared_ptr<SQLite::Database> &db);
    ~UserSqliteDao();

    /// @brief Login with the entered name and password
    bool login(const std::string &entered_name, const std::string &entered_password) override;

    /// @brief Add a new user to the database
    bool add(const std::shared_ptr<User> &user) override;

    /// @brief Get all users from the database
    std::vector<std::shared_ptr<User>> all() override;

    /// @brief  Remove a user from the database by id
    bool remove(const int &id) override;

    /// @brief  Clear all users from the database
    bool clear() override;

    /// @brief Update a user in the database by id
    bool update(const int &id, const std::shared_ptr<User> &user) override;

    /// @brief Get a user by id and return the user object
    std::shared_ptr<User> get(const int &id) override;

    /// @brief Check if a user with the given name exists in the database.
    bool exists(const std::string &name) override;

    /// @brief Get the current user object
    std::shared_ptr<User> currentUser() override;

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
    int                               current_user_id_;
};