#include "carton_data_mysql_dao.h"
#include "carton_tables.hpp"

#include <memory>
#include <qsettings>
#include <vector>

CartonDataMysqlDao::CartonDataMysqlDao(const std::string &order_name)
    : order_name_(order_name) {
    init();
}

bool CartonDataMysqlDao::batchAdd(const std::vector<std::shared_ptr<CartonData>> &carton_datas, const size_t batch_size) {
    std::vector<CartonTables> rows;
    rows.reserve(carton_datas.size());

    for (auto carton_data : carton_datas) {
        CartonTables row;
        row["filename"]      = carton_data->filename;
        row["carton_number"] = carton_data->carton_number;
        row["start_number"]  = carton_data->start_number;
        row["end_number"]    = carton_data->end_number;
        row["quantity"]      = carton_data->quantity;
        row["start_barcode"] = carton_data->start_barcode;
        row["end_barcode"]   = carton_data->end_barcode;

        rows.emplace_back(std::move(row));

        // 及时释放内存，防止内存泄漏
        if (rows.size() == batch_size) {
            CartonTables(*db_, order_name_, "id").insert(rows);
            rows.clear();
        }
    }

    if (!rows.empty()) {
        CartonTables(*db_, order_name_, "id").insert(rows);
    }

    return true;
}

std::vector<std::shared_ptr<CartonData>> CartonDataMysqlDao::all(const int &status) {
    std::vector<std::shared_ptr<CartonData>> carton_datas;

    std::vector<CartonTables> carton_tables_all;
    if (status == -1) {
        carton_tables_all = CartonTables(*db_, order_name_, "id").all();
    } else {
        carton_tables_all = CartonTables(*db_, order_name_, "id").where("status", status).all();
    }

    for (auto one : carton_tables_all) {
        auto carton_data           = std::make_shared<CartonData>();
        carton_data->id            = one("id").asInt();
        carton_data->filename      = one("filename").asString();
        carton_data->carton_number = one("carton_number").asString();
        carton_data->start_number  = one("start_number").asString();
        carton_data->end_number    = one("end_number").asString();
        carton_data->quantity      = one("quantity").asInt();
        carton_data->start_barcode = one("start_barcode").asString();
        carton_data->end_barcode   = one("end_barcode").asString();
        carton_data->status        = one("status").asInt();

        carton_datas.push_back(carton_data);
    }

    return carton_datas;
}

bool CartonDataMysqlDao::scanned(const std::string &start_barcode) {
    auto carton_data    = get(start_barcode);
    carton_data->status = 1;
    return update(carton_data->id, carton_data);
}

std::shared_ptr<CartonData> CartonDataMysqlDao::get(const std::string &start_barcode) {
    CartonTables carton_table  = CartonTables(*db_, order_name_, "id").where("start_barcode", start_barcode).one();
    auto         carton_data   = std::make_shared<CartonData>();
    carton_data->id            = carton_table("id").asInt();
    carton_data->filename      = carton_table("filename").asString();
    carton_data->carton_number = carton_table("carton_number").asString();
    carton_data->start_number  = carton_table("start_number").asString();
    carton_data->end_number    = carton_table("end_number").asString();
    carton_data->quantity      = carton_table("quantity").asInt();
    carton_data->start_barcode = carton_table("start_barcode").asString();
    carton_data->end_barcode   = carton_table("end_barcode").asString();
    carton_data->status        = carton_table("status").asInt();

    if (carton_data->id == 0) {
        return nullptr;
    }

    return carton_data;
}

bool CartonDataMysqlDao::update(const int &id, std::shared_ptr<CartonData> &carton_data) {
    CartonTables(*db_, order_name_, "id")
        .where("id", id)
        .update({
            {"filename", carton_data->filename},
            {"carton_number", carton_data->carton_number},
            {"start_number", carton_data->start_number},
            {"end_number", carton_data->end_number},
            {"quantity", carton_data->quantity},
            {"start_barcode", carton_data->start_barcode},
            {"end_barcode", carton_data->end_barcode},
            {"status", carton_data->status},
        });

    return true;
}

bool CartonDataMysqlDao::clear() {
    std::string sql = "DROP TABLE  IF EXISTS `" + order_name_ + "`";
    db_->execute(sql);

    return true;
}

void CartonDataMysqlDao::init() {
    // 读取配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("mysql");
    std::string host     = settings.value("host").toString().toStdString();
    int         port     = settings.value("port").toInt();
    std::string user     = settings.value("user").toString().toStdString();
    std::string password = settings.value("password").toString().toStdString();
    std::string database = "carton_data";
    settings.endGroup();

    db_ = std::make_shared<zel::myorm::Database>();
    if (!db_->connect(host, port, user, password, database)) {
        return;
    }

    if (!CartonTables(*db_, order_name_, "id").exists()) {
        std::string sql = "CREATE TABLE IF NOT EXISTS  `" + order_name_ +
            "` ("
            "id INT AUTO_INCREMENT PRIMARY KEY,"
            "filename VARCHAR(255) NOT NULL,"
            "carton_number VARCHAR(255) NOT NULL,"
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