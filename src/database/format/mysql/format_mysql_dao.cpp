#include "format_mysql_dao.h"

#include "formats.hpp"

#include <memory>

FormatMysqlDao::FormatMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

bool FormatMysqlDao::add(const std::shared_ptr<Format> &format) {
    auto formats            = Formats(*db_);
    formats["name"]         = format->name;
    formats["type"]         = format->type;
    formats["filename"]     = format->filename;
    formats["box_number"]   = format->box_number;
    formats["start_number"] = format->start_number;
    formats["end_number"]   = format->end_number;
    formats["quantity"]     = format->quantity;
    formats["barcode"]      = format->barcode;

    return formats.save();
}

std::vector<std::shared_ptr<Format>> FormatMysqlDao::all() {
    std::vector<std::shared_ptr<Format>> formats;

    auto all = Formats(*db_).all();
    for (auto one : all) {
        std::shared_ptr<Format> format = std::make_shared<Format>();
        format->id                     = one("id").asInt();
        format->name                   = one("name").asString();
        format->type                   = one("type").asInt();
        format->filename               = one("filename").asString();
        format->box_number             = one("box_number").asString();
        format->start_number           = one("start_number").asString();
        format->end_number             = one("end_number").asString();
        format->quantity               = one("quantity").asString();
        format->barcode                = one("barcode").asString();

        formats.push_back(format);
    }

    return formats;
}

std::shared_ptr<Format> FormatMysqlDao::get(const int id) {
    auto one = Formats(*db_).where("id", id).one();

    std::shared_ptr<Format> format = std::make_shared<Format>();
    format->id                     = one("id").asInt();
    format->name                   = one("name").asString();
    format->type                   = one("type").asInt();
    format->filename               = one("filename").asString();
    format->box_number             = one("box_number").asString();
    format->start_number           = one("start_number").asString();
    format->end_number             = one("end_number").asString();
    format->quantity               = one("quantity").asString();
    format->barcode                = one("barcode").asString();

    return format;
}

void FormatMysqlDao::init() {
    // check if not exists
    if (!Formats(*db_).exists()) {
        // create if not exists
        std::string sql = "CREATE TABLE IF NOT EXISTS formats ("
                          "id INT AUTO_INCREMENT PRIMARY KEY,"
                          "name VARCHAR(255) NOT NULL,"
                          "type INT NOT NULL,"
                          "filename VARCHAR(255) NOT NULL,"
                          "box_number VARCHAR(255) NOT NULL,"
                          "start_number VARCHAR(255) NOT NULL,"
                          "end_number VARCHAR(255) NOT NULL,"
                          "quantity VARCHAR(255) NOT NULL,"
                          "barcode VARCHAR(255) NOT NULL"
                          ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;";
        db_->execute(sql);

        // initialize format data
        std::shared_ptr<Format> cq01_box    = std::make_shared<Format>(Format{0, "CQ01", 1, "", "盒号", "J", "K", "L", "二维码"});
        std::shared_ptr<Format> cq01_carton = std::make_shared<Format>(Format{0, "CQ01", 2, "", "箱号", "J", "K", "L", "二维码"});
        std::shared_ptr<Format> cq02_card   = std::make_shared<Format>(Format{0, "CQ02", 3, "", "数量", "条形码ICCID", "条形码IMSI", "卡总数量", "二维码"});
        std::shared_ptr<Format> cq03_box    = std::make_shared<Format>(Format{0, "CQ03", 1, "", "盒号", "开始", "结束", "数量", "二维码"});
        std::shared_ptr<Format> cq03_carton = std::make_shared<Format>(Format{0, "CQ03", 2, "", "箱号", "开始", "结束", "数量", "二维码"});
        std::shared_ptr<Format> surabaya_box =
            std::make_shared<Format>(Format{0, "Surabaya", 1, "No.WO/OF", "Inner Box No.", "First SN", "Last SN", "Quantity", "Barcode"});
        std::shared_ptr<Format> surabaya_carton =
            std::make_shared<Format>(Format{0, "Surabaya", 2, "No.WO/OF", "Outer Box No.", "First SN", "Last SN", "Quantity", "Barcode"});

        add(cq01_box);
        add(cq01_carton);
        add(cq02_card);
        add(cq03_box);
        add(cq03_carton);
        add(surabaya_box);
        add(surabaya_carton);
    }
}