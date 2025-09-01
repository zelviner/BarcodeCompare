#pragma once

#include "user.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class UserDao {

  public:
    UserDao(const std::shared_ptr<SQLite::Database> &db);
    ~UserDao();

    /// @brief Login with the entered name and password
    bool login(const std::string &entered_name, const std::string &entered_password);

    /// @brief Add a new user to the database
    bool add(const std::shared_ptr<User> &user);

    /// @brief Get all users from the database
    std::vector<std::shared_ptr<User>> all();

    /// @brief  Remove a user from the database by id
    bool remove(const int &id);

    /// @brief  Clear all users from the database
    bool clear();

    /// @brief Update a user in the database by id
    bool update(const int &id, const std::shared_ptr<User> &user);

    /// @brief Get a user by id and return the user object
    std::shared_ptr<User> get(const int &id);

    /// @brief Check if a user with the given name exists in the database.
    bool exists(const std::string &name);

    /// @brief Get the current user object
    std::shared_ptr<User> currentUser();

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
    int                               current_user_id_;
};