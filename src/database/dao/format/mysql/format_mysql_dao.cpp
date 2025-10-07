#include "format_mysql_dao.h"
#include "database/dao/format/format.h"
#include <memory>

FormatMysqlDao::FormatMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

FormatMysqlDao::~FormatMysqlDao() {}

bool FormatMysqlDao::add(const std::shared_ptr<Format> &format) {
    (*formats_)["name"]         = format->name;
    (*formats_)["type"]         = format->type;
    (*formats_)["filename"]     = format->filename;
    (*formats_)["box_number"]   = format->box_number;
    (*formats_)["start_number"] = format->start_number;
    (*formats_)["end_number"]   = format->end_number;
    (*formats_)["quantity"]     = format->quantity;
    (*formats_)["barcode"]      = format->barcode;

    return formats_->save();
}

std::vector<std::shared_ptr<Format>> FormatMysqlDao::all() {
    std::vector<std::shared_ptr<Format>> formats;

    auto all = formats_->all();
    for (auto one : all) {
        std::shared_ptr<Format> format = std::make_shared<Format>();
        format->id                     = one["id"].asInt();
        format->name                   = one["name"].asString();
        format->type                   = one["type"].asInt();
        format->filename               = one["filename"].asString();
        format->box_number             = one["box_number"].asString();
        format->start_number           = one["start_number"].asString();
        format->end_number             = one["end_number"].asString();
        format->quantity               = one["quantity"].asString();
        format->barcode                = one["barcode"].asString();

        formats.push_back(format);
    }

    return formats;
}

std::shared_ptr<Format> FormatMysqlDao::get(const int id) {
    auto one = formats_->where("id", id).one();

    std::shared_ptr<Format> format = std::make_shared<Format>();
    format->id                     = one["id"].asInt();
    format->name                   = one["name"].asString();
    format->type                   = one["type"].asInt();
    format->filename               = one["filename"].asString();
    format->box_number             = one["box_number"].asString();
    format->start_number           = one["start_number"].asString();
    format->end_number             = one["end_number"].asString();
    format->quantity               = one["quantity"].asString();
    format->barcode                = one["barcode"].asString();

    return format;
}

void FormatMysqlDao::init() {
    formats_ = std::make_shared<Formats>(*db_);

    // check if not exists
    if (!formats_->exists()) {
        // create if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS formats ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                          "name VARCHAR(255) NOT NULL UNQIUE,"
                          "type INTEGER NOT NULL,"
                          "filename VARCHAE(255) NOT NULL,"
                          "box_number VARCHAE(255) NOT NULL,"
                          "start_number VARCHAE(255) NOT NULL,"
                          "end_number VARCHAE(255) NOT NULL,"
                          "quantity VARCHAE(255) NOT NULL,"
                          "barcode VARCHAE(255) NOT NULL);";
        db_->execute(sql);

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