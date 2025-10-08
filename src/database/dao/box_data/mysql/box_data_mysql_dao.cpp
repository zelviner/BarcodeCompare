#include "box_data_mysql_dao.h"
#include "box_tables.hpp"

#include <memory>
#include <qsettings>

BoxDataMysqlDao::BoxDataMysqlDao(const std::string &order_name)
    : order_name_(order_name) {
    init();
}
BoxDataMysqlDao::~BoxDataMysqlDao() {}

bool BoxDataMysqlDao::batchAdd(const std::vector<std::shared_ptr<BoxData>> &box_datas) {

    std::vector<BoxTables> rows;
    for (auto box_data : box_datas) {
        BoxTables row;
        row["filename"]      = box_data->filename;
        row["box_number"]    = box_data->box_number;
        row["start_number"]  = box_data->start_number;
        row["end_number"]    = box_data->end_number;
        row["quantity"]      = box_data->quantity;
        row["start_barcode"] = box_data->start_barcode;
        row["end_barcode"]   = box_data->end_barcode;
        row["status"]        = box_data->status;

        rows.push_back(row);
    }

    auto box_tables = BoxTables(*db_, order_name_, "id");
    box_tables.insert(rows);

    return true;
}

std::vector<std::shared_ptr<BoxData>> BoxDataMysqlDao::all(const int &status) {
    std::vector<std::shared_ptr<BoxData>> box_datas;

    std::vector<BoxTables> box_tables_all;
    if (status == -1) {
        box_tables_all = BoxTables(*db_, order_name_, "id").all();
    } else {
        box_tables_all = BoxTables(*db_, order_name_, "id").where("status", status).all();
    }

    for (auto one : box_tables_all) {
        auto box_data           = std::make_shared<BoxData>();
        box_data->id            = one("id").asInt();
        box_data->filename      = one("filename").asString();
        box_data->box_number    = one("box_number").asString();
        box_data->start_number  = one("start_number").asString();
        box_data->end_number    = one("end_number").asString();
        box_data->quantity      = one("quantity").asInt();
        box_data->start_barcode = one("start_barcode").asString();
        box_data->end_barcode   = one("end_barcode").asString();
        box_data->status        = one("status").asInt();

        box_datas.push_back(box_data);
    }

    return box_datas;
}

std::vector<std::shared_ptr<BoxData>> BoxDataMysqlDao::all(const std::string &start_number, const std::string &end_number) {
    std::vector<std::shared_ptr<BoxData>> box_datas;

    std::vector<BoxTables> box_tables_all =
        BoxTables(*db_, order_name_, "id").where("start_number", ">=", start_number).where("end_number", "<=", end_number).all();
    for (auto one : box_tables_all) {
        auto box_data           = std::make_shared<BoxData>();
        box_data->id            = one("id").asInt();
        box_data->filename      = one("filename").asString();
        box_data->box_number    = one("box_number").asString();
        box_data->start_number  = one("start_number").asString();
        box_data->end_number    = one("end_number").asString();
        box_data->quantity      = one("quantity").asInt();
        box_data->start_barcode = one("start_barcode").asString();
        box_data->end_barcode   = one("end_barcode").asString();
        box_data->status        = one("status").asInt();

        box_datas.push_back(box_data);
    }

    return box_datas;
}

bool BoxDataMysqlDao::scanned(const std::string &start_barcode) {
    auto box_data    = get(start_barcode);
    box_data->status = 1;
    return update(box_data->id, box_data);
}

std::shared_ptr<BoxData> BoxDataMysqlDao::get(const std::string &start_barcode) {
    BoxTables box_table     = BoxTables(*db_, order_name_, "id").where("start_barcode", start_barcode).one();
    auto      box_data      = std::make_shared<BoxData>();
    box_data->id            = box_table("id").asInt();
    box_data->filename      = box_table("filename").asString();
    box_data->box_number    = box_table("box_number").asString();
    box_data->start_number  = box_table("start_number").asString();
    box_data->end_number    = box_table("end_number").asString();
    box_data->quantity      = box_table("quantity").asInt();
    box_data->start_barcode = box_table("start_barcode").asString();
    box_data->end_barcode   = box_table("end_barcode").asString();
    box_data->status        = box_table("status").asInt();

    if (box_data->id == 0) {
        return nullptr;
    }

    return box_data;
}

bool BoxDataMysqlDao::update(const int &id, std::shared_ptr<BoxData> &box_data) {
    BoxTables(*db_, order_name_, "id")
        .where("id", id)
        .update({{"filename", box_data->filename},
                 {"box_number", box_data->box_number},
                 {"start_number", box_data->start_number},
                 {"end_number", box_data->end_number},
                 {"quantity", box_data->quantity},
                 {"start_barcode", box_data->start_barcode},
                 {"end_barcode", box_data->end_barcode},
                 {"status", box_data->status}});

    return true;
}

bool BoxDataMysqlDao::clear() {
    std::string sql = "DROP TABLE  IF EXISTS `" + order_name_ + "`";
    db_->execute(sql);

    return true;
}

void BoxDataMysqlDao::init() {
    // 读取配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("mysql");
    std::string host     = settings.value("host").toString().toStdString();
    int         port     = settings.value("port").toInt();
    std::string user     = settings.value("user").toString().toStdString();
    std::string password = settings.value("password").toString().toStdString();
    std::string database = "box_data";
    settings.endGroup();

    db_ = std::make_shared<zel::myorm::Database>();
    if (!db_->connect(host, port, user, password, database)) {
        return;
    }

    auto box_tables = BoxTables(*db_, order_name_, "id");

    if (!box_tables.exists()) {
        std::string sql = "CREATE TABLE IF NOT EXISTS `" + order_name_ +
            "` ("
            "id INT AUTO_INCREMENT PRIMARY KEY,"
            "filename VARCHAR(255) NOT NULL,"
            "box_number VARCHAR(255) NOT NULL,"
            "start_number VARCHAR(255) NOT NULL,"
            "end_number VARCHAR(255) NOT NULL,"
            "quantity INT NOT NULL,"
            "start_barcode VARCHAR(255) NOT NULL,"
            "end_barcode VARCHAR(255) NOT NULL,"
            "status INT NOT NULL DEFAULT 0"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;";

        db_->execute(sql);
    }
}