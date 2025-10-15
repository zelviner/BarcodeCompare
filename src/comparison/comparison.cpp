#include "comparison.h"

#include <memory>
#include <qdebug>
#include <zel/utility/logger.h>

Comparison::Comparison(const std::shared_ptr<Order> &order, const std::shared_ptr<BoxDataDao> &box_data_dao,
                       const std::shared_ptr<CartonDataDao> &carton_data_dao)
    : order_(order)
    , box_data_dao_(box_data_dao)
    , carton_data_dao_(carton_data_dao) {}

Comparison::~Comparison() {}

int Comparison::box(const std::shared_ptr<BoxInfo> &box_info) {

    std::shared_ptr<BoxData> box_data = box_data_dao_->get(box_info->box_start_barcode.toStdString());
    if (box_data == nullptr) {
        log_error("Box start barcode not in order: %s", box_info->box_start_barcode.toStdString().c_str());
        return 1; // 内盒起始条码不在该订单范围内
    }

    QString box_start_barcode  = trim_box_barcode(box_info->box_start_barcode);
    QString box_end_barcode    = trim_box_barcode(box_info->box_end_barcode);
    QString card_start_barcode = trim_card_barcode(box_info->card_start_barcode);
    QString card_end_barcode   = trim_card_barcode(box_info->card_end_barcode);

    if (QString::fromStdString(box_data->end_barcode) != box_end_barcode) {
        log_error("Box end barcode not match: %s vs %s", box_data->end_barcode.c_str(), box_end_barcode.toStdString().c_str());
        return 2; // 不是正确的内盒结束条码
    }

    if (QString::fromStdString(box_data->start_number) != card_start_barcode) {
        log_error("Box start number not match: %s vs %s", box_data->start_number.c_str(), card_start_barcode.toStdString().c_str());
        return 3; // 不是正确的首卡条码
    }

    // 判断尾卡条码是否等于内盒结束条码
    if (QString::fromStdString(box_data->end_number) != card_end_barcode) {
        log_error("Box end number not match: %s vs %s", box_data->end_number.c_str(), card_end_barcode.toStdString().c_str());
        return 4; // 不是正确的尾卡条码
    }

    return 0;
}

int Comparison::carton(const std::shared_ptr<CartonInfo> &carton_info, int &box_widget_id) {
    std::shared_ptr<CartonData> carton_data = carton_data_dao_->get(carton_info->carton_start_barcode.toStdString());
    if (carton_data == nullptr) {
        log_error("Carton start barcode not in order: %s", carton_info->carton_start_barcode.toStdString().c_str());
        return 1; // 外箱起始条码不在该订单范围内
    }

    auto box_datas = box_data_dao_->all(carton_data->start_number, carton_data->end_number);

    QString carton_start_barcode = trim_carton_barcode(carton_info->carton_start_barcode);
    QString carton_end_barcode   = trim_carton_barcode(carton_info->carton_end_barcode);
    QString target_barcode       = trim_box_barcode(carton_info->target_barcode);

    if (QString::fromStdString(carton_data->end_barcode) != carton_end_barcode) {
        log_error("Carton end barcode not match: %s vs %s", carton_data->end_barcode.c_str(), carton_end_barcode.toStdString().c_str());
        return 2; // 不是正确的外箱结束条码
    }

    // 判断目标条码是否在该外箱范围内
    bool not_found = true;
    for (auto box_data : box_datas) {
        if (QString::fromStdString(box_data->start_number) == target_barcode || QString::fromStdString(box_data->end_number) == target_barcode) {
            not_found     = false;
            box_widget_id = box_data->id;
            break;
        }
    }
    if (not_found) {
        log_error("Target barcode not in carton %s: %s", carton_data->carton_number.c_str(), target_barcode.toStdString().c_str());
        return 3; // 目标条码不在该外箱范围内
    }

    return 0;
}

QString Comparison::trim_card_barcode(const QString &card_barcode) {
    int card_start_check_num = order_->card_start_check_num - 1;
    int card_end_check_num   = order_->card_end_check_num;

    return trim_barcode(card_barcode, card_start_check_num, card_end_check_num);
}

QString Comparison::trim_box_barcode(const QString &box_barcode) {
    int box_start_check_num = order_->box_start_check_num - 1;
    int box_end_check_num   = order_->box_end_check_num;

    return trim_barcode(box_barcode, box_start_check_num, box_end_check_num);
}

QString Comparison::trim_carton_barcode(const QString &carton_barcode) {
    int carton_start_check_num = order_->carton_start_check_num - 1;
    int carton_end_check_num   = order_->carton_end_check_num;

    return trim_barcode(carton_barcode, carton_start_check_num, carton_end_check_num);
}

QString Comparison::trim_barcode(const QString &barcode, int start, int end) {
    if (barcode.isEmpty()) {
        return QString();
    }

    if (start < 0) {
        start = 0;
    }

    if (end > barcode.length()) {
        end = barcode.length();
    }

    if (end <= start) {
        return QString();
    }

    return barcode.mid(start, end - start);
}