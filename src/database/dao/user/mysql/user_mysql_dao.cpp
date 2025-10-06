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

bool UserMysqlDao::add(const std::shared_ptr<User> &user) {
    (*users_)["name"]     = user->name;
    (*users_)["password"] = user->password;
    (*users_)["role_id"]  = user->role_id;

    return users_->save();
}

std::vector<std::shared_ptr<User>> UserMysqlDao::all() {
    std::vector<std::shared_ptr<User>> users;

    auto all = users_->all();
    for (auto one : all) {
        std::shared_ptr<User> user = std::make_shared<User>();
        user->id                   = one["id"].asInt();
        user->name                 = one["name"].asString();
        user->password             = one["password"].asString();
        user->role_id              = one["role_id"].asInt();

        users.push_back(user);
    }

    return users;
}

bool UserMysqlDao::remove(const int &id) {
    auto one = users_->where("id", id).one();
    one.remove();

    return true;
}

bool UserMysqlDao::clear() {
    users_->remove();
    return true;
}

bool UserMysqlDao::update(const int &id, const std::shared_ptr<User> &user) {
    auto one        = users_->where("id", id).one();
    one["name"]     = user->name;
    one["password"] = user->password;
    one["role_id"]  = user->role_id;

    return users_->save();
}

std::shared_ptr<User> UserMysqlDao::currentUser() { return get(current_user_id_); }

std::shared_ptr<User> UserMysqlDao::get(const int &id) {
    std::shared_ptr<User> user = std::make_shared<User>();
    auto                  one  = users_->where("id", id).one();
    user->id                   = one["id"].asInt();
    user->name                 = one["name"].asString();
    user->password             = one["password"].asString();
    user->role_id              = one["role_id"].asInt();

    return user;
}

bool UserMysqlDao::exists(const std::string &name) {
    auto one = users_->where("name", name).one()["name"].asString();

    return one == "" ? false : true;
}

void UserMysqlDao::init() {
    std::string sql = "SELECT COUNT(*)"
                      "FROM information_schema.tables"
                      "WHERE table_schema = 'barcode_compare'"
                      "  AND table_name = 'users'";

    users_ = std::make_shared<Users>(*db_);

    // auto qurey = db_->query(sql);
    // if (qurey.size() == 0 || qurey[0]["COUNT(*)"].asInt() == 0) {

    // check if table exists
    if (!users_->exists()) {
        // create table if not exists
        sql = "CREATE TABLE IF NOT EXISTS users ("
              "id integer PRIMARY KEY AUTO_INCREMENT,"
              "name varchar(255) NOT NULL,"
              "password varchar(255) NOT NULL,"
              "role_id integer NOT NULL);";
        db_->execute(sql);

        // initialize user data
        std::shared_ptr<User> admin = std::make_shared<User>(User{1, "admin", "iflogic2025", 1});
        add(admin);
    }
}