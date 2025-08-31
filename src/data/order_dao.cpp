#include "order_dao.h"
#include "box_data_dao.h"
#include "carton_data_dao.h"
#include "data/box_data.h"
#include "utils/excel_importer.h"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Statement.h>
#include <memory>
#include <vector>

OrderDao::OrderDao(const std::shared_ptr<SQLite::Database> &db)
    : db_(db) {
    init();
}

OrderDao::~OrderDao() {}

bool OrderDao::add(const std::shared_ptr<Order> &order) {

    std::shared_ptr<utils::ExcelImporter>    excel_importer = std::make_shared<utils::ExcelImporter>(order->box_file_path, order->carton_file_path);
    std::vector<std::shared_ptr<BoxData>>    box_datas      = excel_importer->boxDatas();
    std::vector<std::shared_ptr<CartonData>> carton_datas   = excel_importer->cartonDatas();

    std::shared_ptr<BoxDataDao> box_data_dao = std::make_shared<BoxDataDao>(db_, order->name);
    if (!box_data_dao->batchAdd(box_datas)) {
        return false;
    }

    std::shared_ptr<CartonDataDao> carton_data_dao = std::make_shared<CartonDataDao>(db_, order->name);
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

bool OrderDao::remove(const int &id) {

    std::shared_ptr<Order>         order           = get(id);
    std::shared_ptr<BoxDataDao>    box_data_dao    = std::make_shared<BoxDataDao>(db_, order->name);
    std::shared_ptr<CartonDataDao> carton_data_dao = std::make_shared<CartonDataDao>(db_, order->name);
    box_data_dao->clear();
    carton_data_dao->clear();

    std::string       sql = "DELETE FROM orders WHERE id = ?";
    SQLite::Statement remove(*db_, sql);
    remove.bind(1, id);

    return remove.exec();
}

bool OrderDao::clear() {
    std::string       sql = "DELETE FROM orders";
    SQLite::Statement clear(*db_, sql);

    return clear.exec();
}

bool OrderDao::update(const int &id, const std::shared_ptr<Order> &order) {
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

std::vector<std::shared_ptr<Order>> OrderDao::all() {
    std::string       sql = "SELECT * FROM orders";
    SQLite::Statement all(*db_, sql);

    std::vector<std::shared_ptr<Order>> orders;
    while (all.executeStep()) {
        std::shared_ptr<Order> order = std::make_shared<Order>();

        order->id                     = all.getColumn(0);
        order->name                   = all.getColumn(1).getString();
        order->check_format           = all.getColumn(2).getString();
        order->carton_start_check_num = all.getColumn(2);
        order->carton_end_check_num   = all.getColumn(4);
        order->box_start_check_num    = all.getColumn(5);
        order->box_end_check_num      = all.getColumn(6);
        order->card_start_check_num   = all.getColumn(7);
        order->card_end_check_num     = all.getColumn(8);
        order->box_scanned_num        = all.getColumn(9);
        order->carton_scanned_num     = all.getColumn(10);
        order->mode_id                = all.getColumn(11);
        order->create_time            = all.getColumn(12).getString();

        orders.push_back(order);
    }

    return orders;
}

std::shared_ptr<Order> OrderDao::get(const int &id) {
    std::string       sql = "SELECT * FROM orders WHERE id = ?";
    SQLite::Statement get(*db_, sql);
    get.bind(1, id);

    if (get.executeStep()) {
        std::shared_ptr<Order> order = std::make_shared<Order>();

        order->id                     = get.getColumn(0);
        order->name                   = get.getColumn(1).getString();
        order->check_format           = get.getColumn(2).getString();
        order->carton_start_check_num = get.getColumn(3);
        order->carton_end_check_num   = get.getColumn(4);
        order->box_start_check_num    = get.getColumn(5);
        order->box_end_check_num      = get.getColumn(6);
        order->card_start_check_num   = get.getColumn(7);
        order->card_end_check_num     = get.getColumn(8);
        order->box_scanned_num        = get.getColumn(9);
        order->carton_scanned_num     = get.getColumn(10);
        order->mode_id                = get.getColumn(11);
        order->create_time            = get.getColumn(12).getString();

        return order;
    }

    return nullptr;
}

bool OrderDao::currentOrder(const int &order_id) {
    current_order_id_ = order_id;
    return true;
}

std::shared_ptr<Order> OrderDao::currentOrder() {
    if (current_order_id_ == 0) {
        return nullptr;
    }

    return get(current_order_id_);
}

void OrderDao::init() {
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