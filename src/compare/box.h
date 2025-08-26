#pragma once

#include "data/order.h"
#include <memory>
#include <qstring>

class Box {

  public:
    struct BoxInfo {
        QString start_barcode;             // 内盒起始条码
        QString end_barcode;               // 内盒结束条码
        QString start_card_barcode;        // 首卡条码
        QString end_card_barcode;          // 尾卡条码
        int     inner_box_start_check_num; // 内盒起始校验位
        int     inner_box_end_check_num;   // 内盒结束校验位
        int     card_start_check_num;      // 首卡校验位
        int     card_end_check_num;        // 尾卡校验位
        int     scanned_num;               // 已扫描数量
    };

    Box(const std::shared_ptr<Order::OrderInfo> &order_info, const std::shared_ptr<BoxInfo> &box_info);

    ~Box();

    /// @brief 比对
    /// @param error 传出参数，错误信息
    bool compare(QString &error);

  private:
    std::shared_ptr<BoxInfo>          box_info_;
    std::shared_ptr<Order::OrderInfo> order_info_;
};