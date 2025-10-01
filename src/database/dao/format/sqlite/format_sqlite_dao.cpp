#include "format_sqlite_dao.h"

#include <SQLiteCpp/Statement.h>

FormatSqliteDao::FormatSqliteDao(const std::shared_ptr<SQLite::Database> &db)
    : db_(db) {
    init();
}

FormatSqliteDao::~FormatSqliteDao() {}

bool FormatSqliteDao::add(const std::shared_ptr<Format> &format) {
    SQLite::Statement insert(*db_,
                             "INSERT INTO formats (name, type, filename, box_number, start_number, end_number, quantity, barcode) VALUES (?,?,?,?,?,?,?,?)");
    insert.bind(1, format->name);
    insert.bind(2, format->type);
    insert.bind(3, format->filename);
    insert.bind(4, format->box_number);
    insert.bind(5, format->start_number);
    insert.bind(6, format->end_number);
    insert.bind(7, format->quantity);
    insert.bind(8, format->barcode);

    return insert.exec();
}

std::vector<std::shared_ptr<Format>> FormatSqliteDao::all() {
    std::string       sql = "SELECT * FROM formats";
    SQLite::Statement query(*db_, sql);

    std::vector<std::shared_ptr<Format>> roles;
    while (query.executeStep()) {
        std::shared_ptr<Format> format = std::make_shared<Format>();
        format->id                     = query.getColumn("id");
        format->name                   = query.getColumn("name").getString();
        format->type                   = query.getColumn("type");
        format->filename               = query.getColumn("filename").getString();
        format->box_number             = query.getColumn("box_number").getString();
        format->start_number           = query.getColumn("start_number").getString();
        format->end_number             = query.getColumn("end_number").getString();
        format->quantity               = query.getColumn("quantity").getString();
        format->barcode                = query.getColumn("barcode").getString();

        roles.push_back(format);
    }

    return roles;
}

std::shared_ptr<Format> FormatSqliteDao::get(const int id) {
    std::string       sql = "SELECT * FROM formats WHERE id = ?";
    SQLite::Statement query(*db_, sql);
    query.bind(1, id);

    if (query.executeStep()) {
        std::shared_ptr<Format> format = std::make_shared<Format>();
        format->id                     = query.getColumn("id");
        format->name                   = query.getColumn("name").getString();
        format->type                   = query.getColumn("type");
        format->filename               = query.getColumn("filename").getString();
        format->box_number             = query.getColumn("box_number").getString();
        format->start_number           = query.getColumn("start_number").getString();
        format->end_number             = query.getColumn("end_number").getString();
        format->quantity               = query.getColumn("quantity").getString();
        format->barcode                = query.getColumn("barcode").getString();

        return format;
    }

    return nullptr;
}

void FormatSqliteDao::init() {

    // check if table exists
    std::string       sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'formats'";
    SQLite::Statement query(*db_, sql);
    if (!query.executeStep()) {
        // create table if not exists
        sql = "CREATE TABLE IF NOT EXISTS formats ("
              "id INTEGER PRIMARY key AUTOINCREMENT,"
              "name TEXT,"
              "type INTEGER,"
              "filename TEXT,"
              "box_number TEXT,"
              "start_number TEXT,"
              "end_number TEXT,"
              "quantity TEXT,"
              "barcode TEXT);";
        db_->exec(sql);

        // initialize format data
        std::shared_ptr<Format> cq01_box    = std::make_shared<Format>(Format{0, "CQ01", 1, "", "盒号", "J", "K", "L", "二维码"});
        std::shared_ptr<Format> cq01_carton = std::make_shared<Format>(Format{0, "CQ01", 2, "", "箱号", "J", "K", "L", "二维码"});
        std::shared_ptr<Format> cq02_box    = std::make_shared<Format>(Format{0, "CQ02", 1, "", "盒号", "J", "K", "L", ""});
        std::shared_ptr<Format> cq02_carton = std::make_shared<Format>(Format{0, "CQ02", 2, "", "箱号", "J", "K", "L", ""});
        std::shared_ptr<Format> surabaya_box =
            std::make_shared<Format>(Format{0, "Surabaya", 1, "No.WO/OF", "Inner Box No.", "First SN", "Last SN", "Quantity", "Barcode"});
        std::shared_ptr<Format> surabaya_carton =
            std::make_shared<Format>(Format{0, "Surabaya", 2, "No.WO/OF", "Outer Box No.", "First SN", "Last SN", "Quantity", "Barcode"});

        add(cq01_box);
        add(cq01_carton);
        add(cq02_box);
        add(cq02_carton);
        add(surabaya_box);
        add(surabaya_carton);
    }
}