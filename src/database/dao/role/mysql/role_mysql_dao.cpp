#include "role_mysql_dao.h"

#include <immintrin.h>
#include <memory>
#include <vector>

RoleMysqlDao::RoleMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

RoleMysqlDao::~RoleMysqlDao() {}

bool RoleMysqlDao::add(const std::shared_ptr<Role> &role) { return false; }

std::vector<std::shared_ptr<Role>> RoleMysqlDao::all() {
    std::vector<std::shared_ptr<Role>> roles;

    return roles;
}

std::shared_ptr<Role> RoleMysqlDao::get(const int id) { return nullptr; }

void RoleMysqlDao::init() {
    roles_ = std::make_shared<Roles>(*db_);

    // check if table exists
    if (!roles_->exists()) {
        // create table if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS roles ("
                          "id INTEGER PRIMARY key AUTOINCREMENT,"
                          "name VARCHAR(255) NOT NULL UNIQUE,"
                          "description VARCHAR(255);";
    }
}