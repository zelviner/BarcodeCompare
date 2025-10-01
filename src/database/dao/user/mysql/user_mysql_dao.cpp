#include "user_mysql_dao.h"

#include <memory>
#include <vector>

UserMysqlDao::UserMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

UserMysqlDao::~UserMysqlDao() {}

bool UserMysqlDao::login(const std::string &entered_name, const std::string &entered_password) {
    for (auto &user : all()) {
        if (user->name == entered_name && user->password == entered_password) {
            current_user_id_ = user->id;
            return true;
        }
    }

    return false;
}

bool UserMysqlDao::add(const std::shared_ptr<User> &user) { return false; }

std::vector<std::shared_ptr<User>> UserMysqlDao::all() {
    std::vector<std::shared_ptr<User>> users;

    return users;
}

bool UserMysqlDao::remove(const int &id) { return false; }

bool UserMysqlDao::clear() { return false; }

bool UserMysqlDao::update(const int &id, const std::shared_ptr<User> &user) { return false; }

std::shared_ptr<User> UserMysqlDao::currentUser() { return get(current_user_id_); }

std::shared_ptr<User> UserMysqlDao::get(const int &id) { return nullptr; }

bool UserMysqlDao::exists(const std::string &name) { return false; }

void UserMysqlDao::init() {}