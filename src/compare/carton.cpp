#include "carton.h"
#include "utils/high_precision.h"

#include <qdebug>

Carton::Carton()
    : correct_start_barcode_("0")
    , correct_end_barcode_("0") {}

Carton::~Carton() {}

bool Carton::compare(QString &error, bool &is_end) {

    is_end = false;

    int start_check_num     = carton_info_->start_check_num - 1;
    int end_check_num       = carton_info_->end_check_num;
    int box_start_check_num = carton_info_->box_start_check_num - 1;
    int box_end_check_num   = carton_info_->box_end_check_num;

    QString start_barcode  = carton_info_->start_barcode.mid(start_check_num, end_check_num - start_check_num);
    QString end_barcode    = carton_info_->end_barcode.mid(start_check_num, end_check_num - start_check_num);
    QString target_barcode = carton_info_->target_barcode.mid(box_start_check_num, box_end_check_num - box_start_check_num);

    // 判断目标条码是否在范围内
    printf("%s %s %s\n", start_barcode.toStdString().c_str(), end_barcode.toStdString().c_str(), target_barcode.toStdString().c_str());
    if (target_barcode < start_barcode || target_barcode > end_barcode) {
        error = QString("内盒不属于该外箱");
        return false;
    }

    // 判断目标条码是否正确
    if (correct_start_barcode_ != "0" && correct_end_barcode_ != "0") {
        if (target_barcode == correct_start_barcode_ || target_barcode == correct_end_barcode_) {
            if (end_barcode == correct_end_barcode_) {
                is_end = true;
            }
        } else {
            error = QString("内盒条码顺序不正确");
            return false;
        }
    }

    // 计算正确的起始条码和结束条
    HighPrecision high_precision; // 高精度计算
    int           interval = high_precision.sub(target_barcode, start_barcode).toInt();
    if (interval % carton_info_->box_count == 0) {
        correct_start_barcode_ = high_precision.add(target_barcode, carton_info_->box_count);
        int temp               = carton_info_->box_count * 2 - 1;
        correct_end_barcode_   = high_precision.add(target_barcode, temp);
    } else {
        correct_start_barcode_ = high_precision.add(target_barcode, 1);
        correct_end_barcode_   = high_precision.add(target_barcode, carton_info_->box_count);
    }

    printf("correct_start_barcode_ %s\n", correct_start_barcode_.toStdString().c_str());
    printf("correct_end_barcode_ %s\n", correct_end_barcode_.toStdString().c_str());

    return true;
}