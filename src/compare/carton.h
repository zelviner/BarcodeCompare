#pragma once

#include <QString>

class Carton {

  public:
    struct CartonInfo {
        QString start_iccid;          // 外箱起始 ICCID
        QString end_iccid;            // 外箱结束 ICCID
        QString target_iccid;         // 目标 ICCID
        int     start_check_num;      // 外箱起始校验位
        int     end_check_num;        // 外箱结束校验位
        int     card_start_check_num; // 卡片起始校验位
        int     card_end_check_num;   // 卡片结束校验位
        int     card_count;           // 卡片数量
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
    QString     correct_start_iccid_; // 正确起始 ICCID
    QString     correct_end_iccid_;   // 正确结束 ICCID
};