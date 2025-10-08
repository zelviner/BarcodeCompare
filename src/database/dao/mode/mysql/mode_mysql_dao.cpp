#include "mode_mysql_dao.h"
#include "modes.hpp"

#include <memory>
#include <vector>

ModeMysqlDao::ModeMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

ModeMysqlDao::~ModeMysqlDao() {}

bool ModeMysqlDao::add(const std::shared_ptr<Mode> &mode) {
    auto modes           = Modes(*db_);
    modes["name"]        = mode->name;
    modes["description"] = mode->description;

    return modes.save();
}

std::vector<std::shared_ptr<Mode>> ModeMysqlDao::all() {
    std::vector<std::shared_ptr<Mode>> modes;

    auto all = Modes(*db_).all();
    for (auto one : all) {
        std::shared_ptr<Mode> mode = std::make_shared<Mode>();
        mode->id                   = one("id").asInt();
        mode->name                 = one("name").asString();
        mode->description          = one("description").asString();

        modes.push_back(mode);
    }

    return modes;
}

std::shared_ptr<Mode> ModeMysqlDao::get(const int &id) {
    auto one = Modes(*db_).where("id", id).one();

    std::shared_ptr<Mode> mode = std::make_shared<Mode>();
    mode->id                   = one("id").asInt();
    mode->name                 = one("name").asString();
    mode->description          = one("description").asString();

    return mode;
}

void ModeMysqlDao::init() {
    // check if not exists
    if (!Modes(*db_).exists()) {
        // create if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS modes ("
                          "id INT AUTO_INCREMENT PRIMARY KEY,"
                          "name VARCHAR(255) NOT NULL UNIQUE,"
                          "description VARCHAR(255) NOT NULL"
                          ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;";
        db_->execute(sql);

        // initialize mode data
        std::shared_ptr<Mode> start_and_end = std::make_shared<Mode>(Mode{0, "Start and End Barcode", "起始与结束条码"});
        std::shared_ptr<Mode> start         = std::make_shared<Mode>(Mode{0, "Start Barcode", "仅有起始条码"});
        std::shared_ptr<Mode> end           = std::make_shared<Mode>(Mode{0, "End Barcode", "仅有结束条码"});

        add(start_and_end);
        add(start);
        add(end);
    }
}
