#pragma once

#include <qstring>

struct CardInfo {
    QString box_start_barcode; // 内盒起始条码
    QString card_barcode;      // 卡片条码
    QString label_barcode;     // 标签条码
};
