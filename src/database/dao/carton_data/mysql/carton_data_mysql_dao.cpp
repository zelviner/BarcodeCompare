#include "carton_data_mysql_dao.h"

#include <exception>
#include <iostream>
#include <memory>
#include <vector>

CartonDataMysqlDao::CartonDataMysqlDao(const std::shared_ptr<zel::myorm::Database> &db, const std::string &order_name)
    : db_(db)
    , order_name_(order_name) {
    init();
}

CartonDataMysqlDao::~CartonDataMysqlDao() {}

bool CartonDataMysqlDao::batchAdd(const std::vector<std::shared_ptr<CartonData>> &carton_datas) { return true; }

std::vector<std::shared_ptr<CartonData>> CartonDataMysqlDao::all(const int &status) {

    std::vector<std::shared_ptr<CartonData>> carton_datas;

    return carton_datas;
}

bool CartonDataMysqlDao::scanned(const std::string &start_barcode) {
    auto carton_data    = get(start_barcode);
    carton_data->status = 1;
    return update(carton_data->id, carton_data);
}

std::shared_ptr<CartonData> CartonDataMysqlDao::get(const std::string &start_barcode) { return nullptr; }

bool CartonDataMysqlDao::update(const int &id, std::shared_ptr<CartonData> &carton_data) { return true; }

bool CartonDataMysqlDao::clear() { return true; }

void CartonDataMysqlDao::init() {}