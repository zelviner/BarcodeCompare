#pragma once

#include <qstring>

struct CartonInfo {
    QString carton_start_or_end_barcode; // 外箱起始或结束条码
    QString target_barcode;              // 目标条码
};
