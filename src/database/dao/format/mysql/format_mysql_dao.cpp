#include "format_mysql_dao.h"

FormatMysqlDao::FormatMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

FormatMysqlDao::~FormatMysqlDao() {}

bool FormatMysqlDao::add(const std::shared_ptr<Format> &format) { return false; }

std::vector<std::shared_ptr<Format>> FormatMysqlDao::all() {
    std::vector<std::shared_ptr<Format>> roles;

    return roles;
}

std::shared_ptr<Format> FormatMysqlDao::get(const int id) { return nullptr; }

void FormatMysqlDao::init() {}