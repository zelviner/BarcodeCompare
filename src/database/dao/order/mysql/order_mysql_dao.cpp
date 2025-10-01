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

    return false;
}

bool OrderMysqlDao::remove(const int &id) {

    std::shared_ptr<Order> order           = get(id);
    auto                   box_data_dao    = BoxDataDaoFactory::create(nullptr, db_, order->name);
    auto                   carton_data_dao = CartonDataDaoFactory::create(nullptr, db_, order->name);
    box_data_dao->clear();
    carton_data_dao->clear();

    return false;
}

bool OrderMysqlDao::clear() {
    std::vector<std::shared_ptr<Order>> orders = all();
    for (auto order : orders) {
        auto box_data_dao    = BoxDataDaoFactory::create(nullptr, db_, order->name);
        auto carton_data_dao = CartonDataDaoFactory::create(nullptr, db_, order->name);
        box_data_dao->clear();
        carton_data_dao->clear();
    }

    return false;
}

bool OrderMysqlDao::update(const int &id, const std::shared_ptr<Order> &order) { return false; }

std::vector<std::shared_ptr<Order>> OrderMysqlDao::all() {
    std::vector<std::shared_ptr<Order>> orders;

    return orders;
}

std::shared_ptr<Order> OrderMysqlDao::get(const int &id) { return nullptr; }

std::shared_ptr<Order> OrderMysqlDao::get(const std::string &name) { return nullptr; }

bool OrderMysqlDao::exists(const std::string &name) { return false; }

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

void OrderMysqlDao::init() {}

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