#include "outer_box.h"
#include "utils/high_precision.h"

#include <QDebug>

OuterBox::OuterBox()
    : correct_start_iccid_("0")
    , correct_end_iccid_("0") {}

OuterBox::~OuterBox() {}

bool OuterBox::compare(QString &error, bool &is_end) {

    is_end = false;

    int start_check_num      = outer_box_info_->start_check_num - 1;
    int end_check_num        = outer_box_info_->end_check_num;
    int card_start_check_num = outer_box_info_->card_start_check_num - 1;
    int card_end_check_num   = outer_box_info_->card_end_check_num;

    QString start_iccid  = outer_box_info_->start_iccid.mid(start_check_num, end_check_num - start_check_num);
    QString end_iccid    = outer_box_info_->end_iccid.mid(start_check_num, end_check_num - start_check_num);
    QString target_iccid = outer_box_info_->target_iccid.mid(card_start_check_num, card_end_check_num - card_start_check_num);

    // 判断目标 ICCID 是否在范围内
    printf("%s %s %s\n", start_iccid.toStdString().c_str(), end_iccid.toStdString().c_str(), target_iccid.toStdString().c_str());
    if (target_iccid < start_iccid || target_iccid > end_iccid) {
        error = QString("内盒不属于该外箱");
        return false;
    }

    // 判断目标 ICCID 是否正确
    if (correct_start_iccid_ != "0" && correct_end_iccid_ != "0") {
        if (target_iccid == correct_start_iccid_ || target_iccid == correct_end_iccid_) {
            if (end_iccid == correct_end_iccid_) {
                is_end = true;
            }
        } else {
            error = QString("内盒 ICCID 顺序不正确");
            return false;
        }
    }

    // 计算正确的起始 ICCID 和结束 ICCID
    HighPrecision high_precision; // 高精度计算
    int           interval = high_precision.sub(target_iccid, start_iccid).toInt();
    if (interval % outer_box_info_->card_count == 0) {
        correct_start_iccid_ = high_precision.add(target_iccid, outer_box_info_->card_count);
        int temp             = outer_box_info_->card_count * 2 - 1;
        correct_end_iccid_   = high_precision.add(target_iccid, temp);
    } else {
        correct_start_iccid_ = high_precision.add(target_iccid, 1);
        correct_end_iccid_   = high_precision.add(target_iccid, outer_box_info_->card_count);
    }

    return true;
}
