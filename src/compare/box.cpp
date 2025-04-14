#include "box.h"

#include <QDebug>

Box::Box(BoxInfo *inner_box_info)
    : box_info_(inner_box_info) {}

Box::~Box() {}

bool Box::compare(QString &error) {

    int inner_box_start_check_num = box_info_->inner_box_start_check_num - 1;
    int inner_box_end_check_num   = box_info_->inner_box_end_check_num;
    int card_start_check_num      = box_info_->card_start_check_num - 1;
    int card_end_check_num        = box_info_->card_end_check_num;

    QString start_iccid      = box_info_->start_iccid.mid(inner_box_start_check_num, inner_box_end_check_num - inner_box_start_check_num);
    QString end_iccid        = box_info_->end_iccid.mid(inner_box_start_check_num, inner_box_end_check_num - inner_box_start_check_num);
    QString start_card_iccid = box_info_->start_card_iccid.mid(card_start_check_num, card_end_check_num - card_start_check_num);
    QString end_card_iccid   = box_info_->end_card_iccid.mid(card_start_check_num, card_end_check_num - card_start_check_num);

    // 判断 check_num 是否大于 start_iccid位数
    if (inner_box_end_check_num > box_info_->start_iccid.size() || inner_box_start_check_num > inner_box_end_check_num) {
        error = "校验位数大于起始ICCID位数";
        return false;
    }

    // 判断首卡 ICCID 是否等于内盒起始 ICCID
    if (start_iccid != start_card_iccid) {
        error = "内盒起始ICCID与首卡ICCID不相等";
        return false;
    }

    // 判断尾卡 ICCID 是否等于内盒结束 ICCID
    if (end_iccid != end_card_iccid) {
        error = "内盒结束ICCID与尾卡ICCID不相等";
        return false;
    }

    return true;
}
