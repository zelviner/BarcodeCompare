#include "role_mysql_dao.h"

#include <immintrin.h>
#include <memory>
#include <vector>

RoleMysqlDao::RoleMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

RoleMysqlDao::~RoleMysqlDao() {}

bool RoleMysqlDao::add(const std::shared_ptr<Role> &role) {
    (*roles_)["name"]        = role->name;
    (*roles_)["description"] = role->description;

    return roles_->save();
}

std::vector<std::shared_ptr<Role>> RoleMysqlDao::all() {
    std::vector<std::shared_ptr<Role>> roles;

    auto all = roles_->all();
    for (auto one : all) {
        std::shared_ptr<Role> role = std::make_shared<Role>();

        role->id          = one["id"].asInt();
        role->name        = one["name"].asString();
        role->description = one["description"].asString();

        roles.push_back(role);
    }

    return roles;
}

std::shared_ptr<Role> RoleMysqlDao::get(const int id) {
    std::shared_ptr<Role> role = std::make_shared<Role>();

    auto one          = roles_->where("id", id).one();
    role->id          = one["id"].asInt();
    role->name        = one["name"].asString();
    role->description = one["description"].asString();

    return role;
}

void RoleMysqlDao::init() {
    roles_ = std::make_shared<Roles>(*db_);

    // check if table exists
    if (!roles_->exists()) {
        // create table if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS roles ("
                          "id INTEGER PRIMARY key AUTOINCREMENT,"
                          "name VARCHAR(255) NOT NULL UNIQUE,"
                          "description VARCHAR(255);";
        db_->execute(sql);

        // initialize role data
        std::shared_ptr<Role> super_admin = std::make_shared<Role>(Role{0, "SuperAdmin", "超级管理员"});
        std::shared_ptr<Role> admin       = std::make_shared<Role>(Role{0, "Admin", "管理员"});
        std::shared_ptr<Role> user        = std::make_shared<Role>(Role{0, "User", "普通用户"});

        add(super_admin);
        add(admin);
        add(user);
    }
}