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
    (*modes_)["name"]        = mode->name;
    (*modes_)["description"] = mode->description;

    return modes_->save();
}

std::vector<std::shared_ptr<Mode>> ModeMysqlDao::all() {
    std::vector<std::shared_ptr<Mode>> modes;

    auto all = modes_->all();
    for (auto one : all) {
        std::shared_ptr<Mode> mode = std::make_shared<Mode>();
        mode->id                   = one["id"].asInt();
        mode->name                 = one["name"].asString();
        mode->description          = one["description"].asString();

        modes.push_back(mode);
    }

    return modes;
}

std::shared_ptr<Mode> ModeMysqlDao::get(const int &id) {
    auto one = modes_->where("id", id).one();

    std::shared_ptr<Mode> mode = std::make_shared<Mode>();
    mode->id                   = one["id"].asInt();
    mode->name                 = one["name"].asString();
    mode->description          = one["description"].asString();

    return mode;
}

void ModeMysqlDao::init() {
    modes_ = std::make_shared<Modes>(*db_);

    // check if not exists
    if (!modes_->exists()) {
        // create if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS modes ("
                          "id integer PRIMARY KEY AUTOINCREMENT,"
                          "name VAECHAR(255) NOT NULL UNIQUE,"
                          "description VAECHAR(255) NOT NULL);";
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
