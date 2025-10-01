#include "mode_mysql_dao.h"

#include <memory>
#include <vector>

ModeMysqlDao::ModeMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

ModeMysqlDao::~ModeMysqlDao() {}

bool ModeMysqlDao::add(const std::shared_ptr<Mode> &mode) { return false; }

std::vector<std::shared_ptr<Mode>> ModeMysqlDao::all() {
    std::vector<std::shared_ptr<Mode>> modes;
    return modes;
}

std::shared_ptr<Mode> ModeMysqlDao::get(const int &id) { return nullptr; }

void ModeMysqlDao::init() {}
