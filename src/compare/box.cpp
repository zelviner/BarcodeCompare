#include "box.h"
#include "data/order.h"
#include "utils/high_precision.h"

#include <qdebug>

Box::Box(const std::shared_ptr<Order::OrderInfo> &order_info, const std::shared_ptr<BoxInfo> &box_info)
    : box_info_(box_info)
    , order_info_(order_info) {}

Box::~Box() {}

bool Box::compare(QString &error) {

    int inner_box_start_check_num = box_info_->inner_box_start_check_num - 1;
    int inner_box_end_check_num   = box_info_->inner_box_end_check_num;
    int card_start_check_num      = box_info_->card_start_check_num - 1;
    int card_end_check_num        = box_info_->card_end_check_num;

    QString start_barcode      = box_info_->start_barcode.mid(inner_box_start_check_num, inner_box_end_check_num - inner_box_start_check_num);
    QString end_barcode        = "";
    QString start_card_barcode = box_info_->start_card_barcode.mid(card_start_check_num, card_end_check_num - card_start_check_num);
    QString end_card_barcode   = box_info_->end_card_barcode.mid(card_start_check_num, card_end_check_num - card_start_check_num);

    switch (order_info_->barcode_mode) {

    case Order::First: {
        HighPrecision high_precision; // 高精度计算
        end_barcode = high_precision.add(start_barcode, order_info_->box_count - 1);
        break;
    }
    case Order::FirstAndEnd:
        box_info_->end_barcode.mid(inner_box_start_check_num, inner_box_end_check_num - inner_box_start_check_num);
        break;

    default:
        box_info_->end_barcode.mid(inner_box_start_check_num, inner_box_end_check_num - inner_box_start_check_num);
        break;
    }

    // 判断 check_num 是否大于 start_barcode 位数
    if (inner_box_end_check_num > box_info_->start_barcode.size() || inner_box_start_check_num > inner_box_end_check_num) {
        error = "校验位数大于起始条码位数";
        return false;
    }

    // 判断首卡条码是否等于内盒起始条码
    if (start_barcode != start_card_barcode) {
        error = "内盒起始条码首卡条码相等";
        return false;
    }

    // 判断尾卡条码是否等于内盒结束条码
    if (end_barcode != end_card_barcode) {
        printf("end_barcode: %s, end_card_barcode: %s\n", end_barcode.toStdString().c_str(), end_card_barcode.toStdString().c_str());
        error = "内盒结束条码尾卡条码相等";
        return false;
    }

    return true;
}
