#pragma once

#include <qstring>

struct BoxInfo {
    QString box_start_or_end_barcode; // 内盒条码
    QString card_start_barcode;       // 首卡条码
    QString card_end_barcode;         // 尾卡条码
};