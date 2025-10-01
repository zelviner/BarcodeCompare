#include "order_sqlite_dao.h"
#include "database/dao/box_data/box_data_dao_factory.h"
#include "database/dao/carton_data/carton_data_dao_factory.h"
#include "database/dao/format/format_dao_factory.h"
#include "importer/importer_factory.hpp"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>
#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

OrderSqliteDao::OrderSqliteDao(const std::shared_ptr<SQLite::Database> &db)
    : db_(db) {
    init();
}

OrderSqliteDao::~OrderSqliteDao() {}

bool OrderSqliteDao::add(const std::shared_ptr<Order> &order) {
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

    auto format_dao = FormatDaoFactory::create(db_, nullptr);
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

    auto box_data_dao = BoxDataDaoFactory::create(db_, nullptr, order->name);
    if (!box_data_dao->batchAdd(box_datas)) {
        return false;
    }

    auto carton_data_dao = CartonDataDaoFactory::create(db_, nullptr, order->name);
    if (!carton_data_dao->batchAdd(carton_datas)) {
        return false;
    }

    std::string       sql = "INSERT INTO orders (name, check_format, carton_start_check_num, carton_end_check_num, box_start_check_num, "
                            "box_end_check_num, card_start_check_num, card_end_check_num, box_scanned_num, carton_scanned_num, mode_id, create_time)"
                            "VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
    SQLite::Statement add(*db_, sql);
    add.bind(1, order->name);
    add.bind(2, order->check_format);
    add.bind(3, order->carton_start_check_num);
    add.bind(4, order->carton_end_check_num);
    add.bind(5, order->box_start_check_num);
    add.bind(6, order->box_end_check_num);
    add.bind(7, order->card_start_check_num);
    add.bind(8, order->card_end_check_num);
    add.bind(9, order->box_scanned_num);
    add.bind(10, order->carton_scanned_num);
    add.bind(11, order->mode_id);
    add.bind(12, order->create_time);

    return add.exec();
}

bool OrderSqliteDao::remove(const int &id) {

    std::shared_ptr<Order> order           = get(id);
    auto                   box_data_dao    = BoxDataDaoFactory::create(db_, nullptr, order->name);
    auto                   carton_data_dao = CartonDataDaoFactory::create(db_, nullptr, order->name);
    box_data_dao->clear();
    carton_data_dao->clear();

    std::string       sql = "DELETE FROM orders WHERE id = ?";
    SQLite::Statement remove(*db_, sql);
    remove.bind(1, id);

    return remove.exec();
}

bool OrderSqliteDao::clear() {
    std::vector<std::shared_ptr<Order>> orders = all();
    for (auto order : orders) {
        auto box_data_dao    = BoxDataDaoFactory::create(db_, nullptr, order->name);
        auto carton_data_dao = CartonDataDaoFactory::create(db_, nullptr, order->name);
        box_data_dao->clear();
        carton_data_dao->clear();
    }

    std::string       sql = "DELETE FROM orders";
    SQLite::Statement clear(*db_, sql);

    return clear.exec();
}

bool OrderSqliteDao::update(const int &id, const std::shared_ptr<Order> &order) {
    std::string       sql = "UPDATE orders SET name = ?, check_format = ?, carton_start_check_num = ?, "
                            "carton_end_check_num = ?, box_start_check_num = ?, box_end_check_num = ?, card_start_check_num = ?, "
                            "card_end_check_num = ?, box_scanned_num = ?, carton_scanned_num = ?, mode_id = ?, create_time = ? WHERE id = ?";
    SQLite::Statement update(*db_, sql);
    update.bind(1, order->name);
    update.bind(2, order->check_format);
    update.bind(3, order->carton_start_check_num);
    update.bind(4, order->carton_end_check_num);
    update.bind(5, order->box_start_check_num);
    update.bind(6, order->box_end_check_num);
    update.bind(7, order->card_start_check_num);
    update.bind(8, order->card_end_check_num);
    update.bind(9, order->box_scanned_num);
    update.bind(10, order->carton_scanned_num);
    update.bind(11, order->mode_id);
    update.bind(12, order->create_time);
    update.bind(13, id);

    return update.exec();
}

std::vector<std::shared_ptr<Order>> OrderSqliteDao::all() {
    std::string       sql = "SELECT * FROM orders";
    SQLite::Statement all(*db_, sql);

    std::vector<std::shared_ptr<Order>> orders;
    while (all.executeStep()) {
        std::shared_ptr<Order> order  = std::make_shared<Order>();
        order->id                     = all.getColumn("id");
        order->name                   = all.getColumn("name").getString();
        order->check_format           = all.getColumn("check_format").getString();
        order->carton_start_check_num = all.getColumn("carton_start_check_num");
        order->carton_end_check_num   = all.getColumn("carton_end_check_num");
        order->box_start_check_num    = all.getColumn("box_start_check_num");
        order->box_end_check_num      = all.getColumn("box_end_check_num");
        order->card_start_check_num   = all.getColumn("card_start_check_num");
        order->card_end_check_num     = all.getColumn("card_end_check_num");
        order->box_scanned_num        = all.getColumn("box_scanned_num");
        order->carton_scanned_num     = all.getColumn("carton_scanned_num");
        order->mode_id                = all.getColumn("mode_id");
        order->create_time            = all.getColumn("create_time").getString();

        orders.push_back(order);
    }

    return orders;
}

std::shared_ptr<Order> OrderSqliteDao::get(const int &id) {
    std::string       sql = "SELECT * FROM orders WHERE id = ?";
    SQLite::Statement get(*db_, sql);
    get.bind(1, id);

    if (get.executeStep()) {
        std::shared_ptr<Order> order  = std::make_shared<Order>();
        order->id                     = get.getColumn("id");
        order->name                   = get.getColumn("name").getString();
        order->check_format           = get.getColumn("check_format").getString();
        order->carton_start_check_num = get.getColumn("carton_start_check_num");
        order->carton_end_check_num   = get.getColumn("carton_end_check_num");
        order->box_start_check_num    = get.getColumn("box_start_check_num");
        order->box_end_check_num      = get.getColumn("box_end_check_num");
        order->card_start_check_num   = get.getColumn("card_start_check_num");
        order->card_end_check_num     = get.getColumn("card_end_check_num");
        order->box_scanned_num        = get.getColumn("box_scanned_num");
        order->carton_scanned_num     = get.getColumn("carton_scanned_num");
        order->mode_id                = get.getColumn("mode_id");
        order->create_time            = get.getColumn("create_time").getString();

        return order;
    }

    return nullptr;
}

std::shared_ptr<Order> OrderSqliteDao::get(const std::string &name) {
    std::string       sql = "SELECT * FROM orders WHERE name = ?";
    SQLite::Statement get(*db_, sql);
    get.bind(1, name);

    if (get.executeStep()) {
        std::shared_ptr<Order> order  = std::make_shared<Order>();
        order->id                     = get.getColumn("id");
        order->name                   = get.getColumn("name").getString();
        order->check_format           = get.getColumn("check_format").getString();
        order->carton_start_check_num = get.getColumn("carton_start_check_num");
        order->carton_end_check_num   = get.getColumn("carton_end_check_num");
        order->box_start_check_num    = get.getColumn("box_start_check_num");
        order->box_end_check_num      = get.getColumn("box_end_check_num");
        order->card_start_check_num   = get.getColumn("card_start_check_num");
        order->card_end_check_num     = get.getColumn("card_end_check_num");
        order->box_scanned_num        = get.getColumn("box_scanned_num");
        order->carton_scanned_num     = get.getColumn("carton_scanned_num");
        order->mode_id                = get.getColumn("mode_id");
        order->create_time            = get.getColumn("create_time").getString();

        return order;
    }

    return nullptr;
}

bool OrderSqliteDao::exists(const std::string &name) {
    std::string       sql = "SELECT name FROM orders WHERE name = ?";
    SQLite::Statement exists(*db_, sql);
    exists.bind(1, name);

    if (exists.executeStep()) {
        return true;
    }
    return false;
}

bool OrderSqliteDao::currentOrder(const int &order_id) {
    current_order_id_ = order_id;
    return true;
}

std::shared_ptr<Order> OrderSqliteDao::currentOrder() {
    if (current_order_id_ == 0) {
        return nullptr;
    }

    return get(current_order_id_);
}

void OrderSqliteDao::init() {
    // check if table exists
    std::string       sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'orders'";
    SQLite::Statement qurey(*db_, sql);
    if (!qurey.executeStep()) {
        // create table if not exists
        sql = "CREATE TABLE IF NOT EXISTS orders ("
              "id integer PRIMARY KEY AUTOINCREMENT,"
              "name TEXT UNIQUE NOT NULL,"
              "check_format TEXT NOT NULL,"
              "carton_start_check_num integer NOT NULL,"
              "carton_end_check_num integer NOT NULL,"
              "box_start_check_num integer NOT NULL,"
              "box_end_check_num integer NOT NULL,"
              "card_start_check_num integer NOT NULL,"
              "card_end_check_num integer NOT NULL,"
              "box_scanned_num integer NOT NULL,"
              "carton_scanned_num integer NOT NULL,"
              "mode_id integer NOT NULL,"
              "create_time TEXT NOT NULL);";
        db_->exec(sql);
    }
}

bool OrderSqliteDao::hasRequiredValues(const std::vector<std::string> &headers, const std::shared_ptr<Format> &format) {
    std::unordered_set<std::string> headerSet(headers.begin(), headers.end());

    bool result = headerSet.count(format->box_number) && headerSet.count(format->start_number) && headerSet.count(format->end_number) &&
        headerSet.count(format->quantity);

    if (format->barcode != "") {
        return result && headerSet.count(format->barcode);
    } else {
        return true;
    }
}