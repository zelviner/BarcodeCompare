#include "mode_dao.h"

#include <SQLiteCpp/Statement.h>
#include <memory>
#include <vector>

ModeDao::ModeDao(const std::shared_ptr<SQLite::Database> &db)
    : db_(db) {
    init();
}

ModeDao::~ModeDao() {}

bool ModeDao::add(const std::shared_ptr<Mode> &mode) {
    std::string       sql = "INSERT INTO modes (name, description) VALUES (?,?)";
    SQLite::Statement add(*db_, sql);
    add.bind(1, mode->name);
    add.bind(2, mode->description);

    return add.exec();
}

std::vector<std::shared_ptr<Mode>> ModeDao::all() {
    std::string       sql = "SELECT * FROM modes";
    SQLite::Statement all(*db_, sql);

    std::vector<std::shared_ptr<Mode>> modes;
    while (all.executeStep()) {
        std::shared_ptr<Mode> mode = std::make_shared<Mode>();
        mode->id          = all.getColumn("id");
        mode->name        = all.getColumn("name").getString();
        mode->description = all.getColumn("description").getString();

        modes.push_back(mode);
    }

    return modes;
}

std::shared_ptr<Mode> ModeDao::get(const int &id) {
    std::string       sql = "SELECT * FROM modes WHERE id = ?";
    SQLite::Statement query(*db_, sql);
    query.bind(1, id);

    if (query.executeStep()) {
        std::shared_ptr<Mode> mode = std::make_shared<Mode>();
        mode->id          = query.getColumn("id");
        mode->name        = query.getColumn("name").getString();
        mode->description = query.getColumn("description").getString();

        return mode;
    }

    return nullptr;
}

void ModeDao::init() {
    // check if not exists
    std::string       sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'modes'";
    SQLite::Statement query(*db_, sql);

    if (!query.executeStep()) {
        // create table if not exists
        sql = "CREATE TABLE IF NOT EXISTS modes ("
              "id integer PRIMARY KEY AUTOINCREMENT,"
              "name TEXT NOT NULL,"
              "description TEXT NOT NULL);";
        db_->exec(sql);

        // initialize mode data
        std::shared_ptr<Mode> start_and_end = std::make_shared<Mode>(Mode{0, "Start and End Barcode", "起始与结束条码"});
        std::shared_ptr<Mode> start         = std::make_shared<Mode>(Mode{0, "Start Barcode", "仅有起始条码"});
        std::shared_ptr<Mode> end           = std::make_shared<Mode>(Mode{0, "End Barcode", "仅有结束条码"});

        add(start_and_end);
        add(start);
        add(end);
    }
}
