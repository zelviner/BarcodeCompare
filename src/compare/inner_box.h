#pragma once

#include <QString>

class InnerBox {

  public:
    struct InnerBoxInfo {
        QString start_iccid;               // 内盒起始 ICCID
        QString end_iccid;                 // 内盒结束 ICCID
        QString start_card_iccid;          // 首卡 ICCID
        QString end_card_iccid;            // 尾卡 ICCID
        int     inner_box_start_check_num; // 内盒起始校验位
        int     inner_box_end_check_num;   // 内盒结束校验位
        int     card_start_check_num;      // 首卡校验位
        int     card_end_check_num;        // 尾卡校验位
        int     scanned_num;               // 已扫描数量
    };

    InnerBox(InnerBoxInfo *inner_box_info);

    ~InnerBox();

    /// @brief 比对
    /// @param error 传出参数，错误信息
    bool compare(QString &error);

  private:
    InnerBoxInfo *inner_box_info_;
};