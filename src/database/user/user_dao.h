#pragma once

#include "user.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class UserDao {
  public:
    /// @brief Login with the entered name and password
    virtual bool login(const std::string &entered_name, const std::string &entered_password) = 0;

    /// @brief Add a new user to the database
    virtual bool add(const std::shared_ptr<User> &user) = 0;

    /// @brief Get all users from the database
    virtual std::vector<std::shared_ptr<User>> all() = 0;

    /// @brief  Remove a user from the database by id
    virtual bool remove(const int &id) = 0;

    /// @brief  Clear all users from the database
    virtual bool clear() = 0;

    /// @brief Update a user in the database by id
    virtual bool update(const int &id, const std::shared_ptr<User> &user) = 0;

    /// @brief Get a user by id and return the user object
    virtual std::shared_ptr<User> get(const int &id) = 0;

    /// @brief Check if a user with the given name exists in the database.
    virtual bool exists(const std::string &name) = 0;

    /// @brief Get the current user object
    virtual std::shared_ptr<User> currentUser() = 0;
};