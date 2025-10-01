#include "box_data_mysql_dao.h"

#include <exception>
#include <iostream>
#include <memory>

BoxDataMysqlDao::BoxDataMysqlDao(const std::shared_ptr<zel::myorm::Database> &db, const std::string &order_name)
    : db_(db)
    , order_name_(order_name) {
    init();
}
BoxDataMysqlDao::~BoxDataMysqlDao() {}

bool BoxDataMysqlDao::batchAdd(const std::vector<std::shared_ptr<BoxData>> &box_datas) { return true; }

std::vector<std::shared_ptr<BoxData>> BoxDataMysqlDao::all(const int &status) {
    std::vector<std::shared_ptr<BoxData>> box_datas;

    return box_datas;
}

std::vector<std::shared_ptr<BoxData>> BoxDataMysqlDao::all(const std::string &start_number, const std::string &end_number) {
    std::vector<std::shared_ptr<BoxData>> box_datas;

    return box_datas;
}

bool BoxDataMysqlDao::scanned(const std::string &start_barcode) {
    auto box_data    = get(start_barcode);
    box_data->status = 1;
    return update(box_data->id, box_data);
}

std::shared_ptr<BoxData> BoxDataMysqlDao::get(const std::string &start_barcode) { return nullptr; }

bool BoxDataMysqlDao::update(const int &id, std::shared_ptr<BoxData> &box_data) { return true; }

bool BoxDataMysqlDao::clear() { return true; }

void BoxDataMysqlDao::init() {}