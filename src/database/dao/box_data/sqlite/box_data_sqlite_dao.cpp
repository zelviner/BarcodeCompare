#include "box_data_sqlite_dao.h"

#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/Transaction.h>
#include <exception>
#include <iostream>
#include <memory>

BoxDataSqliteDao::BoxDataSqliteDao(const std::shared_ptr<SQLite::Database> &db, const std::string &order_name)
    : db_(db)
    , order_name_(order_name) {
    init();
}
BoxDataSqliteDao::~BoxDataSqliteDao() {}

bool BoxDataSqliteDao::batchAdd(const std::vector<std::shared_ptr<BoxData>> &box_datas) {
    try {
        SQLite::Transaction transaction(*db_);
        SQLite::Statement   query(*db_,
                                  "INSERT INTO box_data.[" + order_name_ +
                                      "] (filename,box_number, start_number, end_number, quantity, start_barcode, end_barcode) VALUES (?,?,?,?,?,?,?)");
        for (const std::shared_ptr<BoxData> &box_data : box_datas) {
            query.bind(1, box_data->filename);
            query.bind(2, box_data->box_number);
            query.bind(3, box_data->start_number);
            query.bind(4, box_data->end_number);
            query.bind(5, box_data->quantity);
            query.bind(6, box_data->start_barcode);
            query.bind(7, box_data->end_barcode);
            query.exec();

            query.reset();
        }

        transaction.commit();
    } catch (std::exception &e) {
        std::cerr << "Batch insert failed: " << e.what() << std::endl;
        throw;
        return false;
    }

    return true;
}

std::vector<std::shared_ptr<BoxData>> BoxDataSqliteDao::all(const int &status) {
    std::string sql;
    if (status == -1) {
        sql = "SELECT * FROM box_data.[" + order_name_ + "]";
    } else {
        sql = "SELECT * FROM box_data.[" + order_name_ + "] WHERE status = ?";
    }

    SQLite::Statement all(*db_, sql);
    if (status != -1) {
        all.bind(1, status);
    }

    std::vector<std::shared_ptr<BoxData>> box_datas;
    while (all.executeStep()) {
        std::shared_ptr<BoxData> box_data = std::make_shared<BoxData>();
        box_data->id                      = all.getColumn("id");
        box_data->filename                = all.getColumn("filename").getString();
        box_data->box_number              = all.getColumn("box_number").getString();
        box_data->start_number            = all.getColumn("start_number").getString();
        box_data->end_number              = all.getColumn("end_number").getString();
        box_data->quantity                = all.getColumn("quantity").getInt();
        box_data->start_barcode           = all.getColumn("start_barcode").getString();
        box_data->end_barcode             = all.getColumn("end_barcode").getString();
        box_data->status                  = all.getColumn("status");

        box_datas.push_back(box_data);
    }

    return box_datas;
}

std::vector<std::shared_ptr<BoxData>> BoxDataSqliteDao::all(const std::string &start_number, const std::string &end_number) {
    std::string       sql = "SELECT * FROM box_data.[" + order_name_ + "] WHERE start_number >= ? AND end_number <= ?";
    SQLite::Statement all(*db_, sql);
    all.bind(1, start_number);
    all.bind(2, end_number);

    std::vector<std::shared_ptr<BoxData>> box_datas;
    while (all.executeStep()) {
        std::shared_ptr<BoxData> box_data = std::make_shared<BoxData>();
        box_data->id                      = all.getColumn("id");
        box_data->filename                = all.getColumn("filename").getString();
        box_data->box_number              = all.getColumn("box_number").getString();
        box_data->start_number            = all.getColumn("start_number").getString();
        box_data->end_number              = all.getColumn("end_number").getString();
        box_data->quantity                = all.getColumn("quantity").getInt();
        box_data->start_barcode           = all.getColumn("start_barcode").getString();
        box_data->end_barcode             = all.getColumn("end_barcode").getString();
        box_data->status                  = all.getColumn("status");

        box_datas.push_back(box_data);
    }

    return box_datas;
}

bool BoxDataSqliteDao::scanned(const std::string &start_barcode) {
    auto box_data    = get(start_barcode);
    box_data->status = 1;
    return update(box_data->id, box_data);
}

std::shared_ptr<BoxData> BoxDataSqliteDao::get(const std::string &start_barcode) {
    std::string       sql = "SELECT * FROM box_data.[" + order_name_ + "] WHERE start_barcode = ?";
    SQLite::Statement get(*db_, sql);
    get.bind(1, start_barcode);

    if (get.executeStep()) {
        std::shared_ptr<BoxData> box_data = std::make_shared<BoxData>();
        box_data->id                      = get.getColumn("id");
        box_data->filename                = get.getColumn("filename").getString();
        box_data->box_number              = get.getColumn("box_number").getString();
        box_data->start_number            = get.getColumn("start_number").getString();
        box_data->end_number              = get.getColumn("end_number").getString();
        box_data->quantity                = get.getColumn("quantity").getInt();
        box_data->start_barcode           = get.getColumn("start_barcode").getString();
        box_data->end_barcode             = get.getColumn("end_barcode").getString();
        box_data->status                  = get.getColumn("status");

        return box_data;
    }

    return nullptr;
}

bool BoxDataSqliteDao::update(const int &id, std::shared_ptr<BoxData> &box_data) {
    try {
        std::string sql = "UPDATE box_data.[" + order_name_ +
            "] SET filename = ?,box_number = ?, start_number = ?, end_number = ?, quantity = ?, start_barcode = ?, end_barcode = ?, status = ? WHERE id = ?";
        SQLite::Statement update(*db_, sql);
        update.bind(1, box_data->filename);
        update.bind(2, box_data->box_number);
        update.bind(3, box_data->start_number);
        update.bind(4, box_data->end_number);
        update.bind(5, box_data->quantity);
        update.bind(6, box_data->start_barcode);
        update.bind(7, box_data->end_barcode);
        update.bind(8, box_data->status);
        update.bind(9, id);

        return update.exec();
    } catch (std::exception &e) {
        std::cerr << "Update box data failed: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool BoxDataSqliteDao::clear() {
    std::string sql = "DROP TABLE  IF EXISTS box_data.[" + order_name_ + "]";
    return db_->exec(sql);
}

void BoxDataSqliteDao::init() {
    std::string       sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'box_data." + order_name_ + "'";
    SQLite::Statement query(*db_, sql);

    if (!query.executeStep()) {
        sql = "CREATE TABLE IF NOT EXISTS box_data.[" + order_name_ +
            "] (id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "filename TEXT,"
            "box_number TEXT,"
            "start_number TEXT,"
            "end_number TEXT,"
            "quantity INTEGER,"
            "start_barcode TEXT,"
            "end_barcode TEXT,"
            "status INTEGER DEFAULT 0)";

        db_->exec(sql);
    }
}