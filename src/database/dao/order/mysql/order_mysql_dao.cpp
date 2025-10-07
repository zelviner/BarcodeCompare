#include "order_mysql_dao.h"
#include "database/dao/box_data/box_data_dao_factory.h"
#include "database/dao/carton_data/carton_data_dao_factory.h"
#include "database/dao/format/format_dao_factory.h"
#include "importer/importer_factory.hpp"

#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

OrderMysqlDao::OrderMysqlDao(const std::shared_ptr<zel::myorm::Database> &db)
    : db_(db) {
    init();
}

OrderMysqlDao::~OrderMysqlDao() {}

bool OrderMysqlDao::add(const std::shared_ptr<Order> &order) {
    size_t                    index          = order->box_file_path.rfind(".");
    std::string               file_extension = order->box_file_path.substr(index, order->box_file_path.size() - index);
    std::shared_ptr<Importer> importer;

    // 根据不同类型文件选择不同的导入器
    if (file_extension == ".xlsx") {
        importer = ImporterFactory::create(ImporterFactory::FileType::XLSX, order->box_file_path, order->carton_file_path);
    } else if (file_extension == ".csv") {
        importer = ImporterFactory::create(ImporterFactory::FileType::CSV, order->box_file_path, order->carton_file_path);
    } else {
        return false;
    }

    auto                                     box_headers    = importer->boxHeaders();
    auto                                     carton_headers = importer->cartonHeaders();
    std::vector<std::shared_ptr<BoxData>>    box_datas;
    std::vector<std::shared_ptr<CartonData>> carton_datas;

    auto format_dao = FormatDaoFactory::create(nullptr, db_);
    auto formats    = format_dao->all();

    // 查看标签数据文件格式是否存在于数据库中
    for (auto format : formats) {
        switch (format->type) {
        case 1:
            if (hasRequiredValues(box_headers, format)) box_datas = importer->boxDatas(format);
            break;

        case 2:
            if (hasRequiredValues(carton_headers, format)) carton_datas = importer->cartonDatas(format);
            break;
        }
    }

    // 找不到预设文件头信息
    if (box_datas.size() == 0 || carton_datas.size() == 0) {
        return false;
    }

    auto box_data_dao = BoxDataDaoFactory::create(nullptr, db_, order->name);
    if (!box_data_dao->batchAdd(box_datas)) {
        return false;
    }

    auto carton_data_dao = CartonDataDaoFactory::create(nullptr, db_, order->name);
    if (!carton_data_dao->batchAdd(carton_datas)) {
        return false;
    }

    (*orders_)["name"]                   = order->name;
    (*orders_)["check_format"]           = order->check_format;
    (*orders_)["carton_start_check_num"] = order->carton_start_check_num;
    (*orders_)["carton_end_check_num"]   = order->carton_end_check_num;
    (*orders_)["box_start_check_num"]    = order->box_start_check_num;
    (*orders_)["box_end_check_num"]      = order->box_end_check_num;
    (*orders_)["card_start_check_num"]   = order->card_start_check_num;
    (*orders_)["card_end_check_num"]     = order->card_end_check_num;
    (*orders_)["box_scanned_num"]        = order->box_scanned_num;
    (*orders_)["carton_scanned_num"]     = order->carton_scanned_num;
    (*orders_)["mode_id"]                = order->mode_id;
    (*orders_)["create_time"]            = order->create_time;
    orders_->save();

    return true;
}

bool OrderMysqlDao::remove(const int &id) {

    std::shared_ptr<Order> order           = get(id);
    auto                   box_data_dao    = BoxDataDaoFactory::create(nullptr, db_, order->name);
    auto                   carton_data_dao = CartonDataDaoFactory::create(nullptr, db_, order->name);
    box_data_dao->clear();
    carton_data_dao->clear();

    auto one = orders_->where("id", id).one();
    one.remove();

    return true;
}

bool OrderMysqlDao::clear() {
    std::vector<std::shared_ptr<Order>> orders = all();
    for (auto order : orders) {
        auto box_data_dao    = BoxDataDaoFactory::create(nullptr, db_, order->name);
        auto carton_data_dao = CartonDataDaoFactory::create(nullptr, db_, order->name);
        box_data_dao->clear();
        carton_data_dao->clear();
    }

    orders_->remove();

    return true;
}

bool OrderMysqlDao::update(const int &id, const std::shared_ptr<Order> &order) {

    auto one = orders_->where("id", id).one();

    one["name"]                   = order->name;
    one["check_format"]           = order->check_format;
    one["carton_start_check_num"] = order->carton_start_check_num;
    one["carton_end_check_num"]   = order->carton_end_check_num;
    one["box_start_check_num"]    = order->box_start_check_num;
    one["box_end_check_num"]      = order->box_end_check_num;
    one["card_start_check_num"]   = order->card_start_check_num;
    one["card_end_check_num"]     = order->card_end_check_num;
    one["box_scanned_num"]        = order->box_scanned_num;
    one["carton_scanned_num"]     = order->carton_scanned_num;
    one["mode_id"]                = order->mode_id;
    one["create_time"]            = order->create_time;

    return one.save();
}

std::vector<std::shared_ptr<Order>> OrderMysqlDao::all() {
    std::vector<std::shared_ptr<Order>> orders;

    auto all = orders_->all();
    for (auto one : all) {
        std::shared_ptr<Order> order  = std::make_shared<Order>();
        order->id                     = one["id"].asInt();
        order->name                   = one["name"].asString();
        order->check_format           = one["check_format"].asString();
        order->carton_start_check_num = one["carton_start_check_num"].asInt();
        order->carton_end_check_num   = one["carton_end_check_num"].asInt();
        order->box_start_check_num    = one["box_start_check_num"].asInt();
        order->box_end_check_num      = one["box_end_check_num"].asInt();
        order->card_start_check_num   = one["card_start_check_num"].asInt();
        order->card_end_check_num     = one["card_end_check_num"].asInt();
        order->box_scanned_num        = one["box_scanned_num"].asInt();
        order->mode_id                = one["mode_id"].asInt();
        order->create_time            = one["create_time"].asString();

        orders.push_back(order);
    }

    return orders;
}

std::shared_ptr<Order> OrderMysqlDao::get(const int &id) {
    std::shared_ptr<Order> order = std::make_shared<Order>();

    auto one                      = orders_->where("id", id).one();
    order->id                     = one["id"].asInt();
    order->name                   = one["name"].asString();
    order->check_format           = one["check_format"].asString();
    order->carton_start_check_num = one["carton_start_check_num"].asInt();
    order->carton_end_check_num   = one["carton_end_check_num"].asInt();
    order->box_start_check_num    = one["box_start_check_num"].asInt();
    order->box_end_check_num      = one["box_end_check_num"].asInt();
    order->card_start_check_num   = one["card_start_check_num"].asInt();
    order->card_end_check_num     = one["card_end_check_num"].asInt();
    order->box_scanned_num        = one["box_scanned_num"].asInt();
    order->mode_id                = one["mode_id"].asInt();
    order->create_time            = one["create_time"].asString();

    return order;
}

std::shared_ptr<Order> OrderMysqlDao::get(const std::string &name) {
    std::shared_ptr<Order> order = std::make_shared<Order>();

    auto one                      = orders_->where("name", name).one();
    order->id                     = one["id"].asInt();
    order->name                   = one["name"].asString();
    order->check_format           = one["check_format"].asString();
    order->carton_start_check_num = one["carton_start_check_num"].asInt();
    order->carton_end_check_num   = one["carton_end_check_num"].asInt();
    order->box_start_check_num    = one["box_start_check_num"].asInt();
    order->box_end_check_num      = one["box_end_check_num"].asInt();
    order->card_start_check_num   = one["card_start_check_num"].asInt();
    order->card_end_check_num     = one["card_end_check_num"].asInt();
    order->box_scanned_num        = one["box_scanned_num"].asInt();
    order->mode_id                = one["mode_id"].asInt();
    order->create_time            = one["create_time"].asString();

    return order;
}

bool OrderMysqlDao::exists(const std::string &name) {
    auto one = orders_->where("name", name).one()["name"].asString();
    return one == "" ? false : true;
}

bool OrderMysqlDao::currentOrder(const int &order_id) {
    current_order_id_ = order_id;
    return true;
}

std::shared_ptr<Order> OrderMysqlDao::currentOrder() {
    if (current_order_id_ == 0) {
        return nullptr;
    }

    return get(current_order_id_);
}

void OrderMysqlDao::init() {
    orders_ = std::make_shared<Orders>(*db_);

    // check if table exists
    if (!orders_->exists()) {
        std::string sql = "CREATE TABLE IF NOT EXISTS orders ("
                          "id integer PRIMARY KEY AUTOINCREMENT,"
                          "name VARCHAR(255) NOT NULL UNIQUE,"
                          "check_format VARCHAR(255) NOT NULL,"
                          "carton_start_check_num integer NOT NULL,"
                          "carton_end_check_num integer NOT NULL,"
                          "box_start_check_num integer NOT NULL,"
                          "box_end_check_num integer NOT NULL,"
                          "card_start_check_num integer NOT NULL,"
                          "card_end_check_num integer NOT NULL,"
                          "box_scanned_num integer NOT NULL,"
                          "carton_scanned_num integer NOT NULL,"
                          "mode_id integer NOT NULL,"
                          "create_time DATETIME NOT NULL);";
        db_->execute(sql);
    }
}

bool OrderMysqlDao::hasRequiredValues(const std::vector<std::string> &headers, const std::shared_ptr<Format> &format) {
    std::unordered_set<std::string> headerSet(headers.begin(), headers.end());

    bool result = headerSet.count(format->box_number) && headerSet.count(format->start_number) && headerSet.count(format->end_number) &&
        headerSet.count(format->quantity);

    if (format->barcode != "") {
        return result && headerSet.count(format->barcode);
    } else {
        return true;
    }
}