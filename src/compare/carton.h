#pragma once

#include <qstring>

class Carton {

  public:
    struct CartonInfo {
        QString start_barcode;       // 外箱起始条码
        QString end_barcode;         // 外箱结束条码
        QString target_barcode;      // 目标条码
        int     start_check_num;     // 外箱起始校验位
        int     end_check_num;       // 外箱结束校验位
        int     box_start_check_num; // 内盒起始校验位
        int     box_end_check_num;   // 内盒结束校验位
        int     box_count;           // 每盒卡片数量
    };

    Carton();
    ~Carton();

    inline void cartonInfo(CartonInfo *carton_info) { carton_info_ = carton_info; }

    /// @brief 比对
    /// @param error 传出参数，错误信息
    /// @param is_end 传出参数，是否结束
    bool compare(QString &error, bool &is_end);

  private:
    CartonInfo *carton_info_;
    QString     correct_start_barcode_; // 正确起始条码
    QString     correct_end_barcode_;   // 正确结束条码
};