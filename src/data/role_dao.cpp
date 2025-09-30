#include "role_dao.h"
#include "role.h"
#include <SQLiteCpp/Statement.h>
#include <immintrin.h>
#include <memory>
#include <vector>

RoleDao::RoleDao(const std::shared_ptr<SQLite::Database> &db)
    : db_(db) {
    init();
}

RoleDao::~RoleDao() {}

bool RoleDao::add(const std::shared_ptr<Role> &role) {
    SQLite::Statement insert(*db_, "INSERT INTO roles (name, description) VALUES (?,?)");
    insert.bind(1, role->name);
    insert.bind(2, role->description);

    return insert.exec();
}

std::vector<std::shared_ptr<Role>> RoleDao::all() {
    std::string       sql = "SELECT * FROM roles";
    SQLite::Statement all(*db_, sql);

    std::vector<std::shared_ptr<Role>> roles;
    while (all.executeStep()) {
        std::shared_ptr<Role> role = std::make_shared<Role>();
        role->id          = all.getColumn("id");
        role->name        = all.getColumn("name").getString();
        role->description = all.getColumn("description").getString();

        roles.push_back(role);
    }

    return roles;
}

std::shared_ptr<Role> RoleDao::get(const int id) {
    std::string       sql = "SELECT * FROM roles WHERE id = ?";
    SQLite::Statement get(*db_, sql);
    get.bind(1, id);

    if (get.executeStep()) {
        std::shared_ptr<Role> role = std::make_shared<Role>();
        role->id          = get.getColumn("id");
        role->name        = get.getColumn("name").getString();
        role->description = get.getColumn("description").getString();

        return role;
    }

    return nullptr;
}

void RoleDao::init() {

    // check if table exists
    std::string       sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'roles'";
    SQLite::Statement query(*db_, sql);
    if (!query.executeStep()) {
        // create table if not exists
        sql = "CREATE TABLE IF NOT EXISTS roles ("
              "id INTEGER PRIMARY key AUTOINCREMENT,"
              "name TEXT,"
              "description TEXT);";
        db_->exec(sql);

        // initialize role data
        std::shared_ptr<Role> super_admin = std::make_shared<Role>(Role{0, "SuperAdmin", "超级管理员"});
        std::shared_ptr<Role> admin       = std::make_shared<Role>(Role{0, "Admin", "管理员"});
        std::shared_ptr<Role> user        = std::make_shared<Role>(Role{0, "User", "普通用户"});

        add(super_admin);
        add(admin);
        add(user);
    }
}