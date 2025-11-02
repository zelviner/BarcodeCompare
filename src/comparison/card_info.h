#pragma once

#include <qstring>

struct CardInfo {
    QString box_start_or_end_barcode; // 内盒起始或结束条码
    QString card_barcode;             // 卡片条码
    QString label_barcode;            // 标签条码
};
