#pragma once

#include <QString>

/// @brief 高精度运算
class HighPrecision {

  public:
    HighPrecision();
    ~HighPrecision();

    /// @brief 加法
    /// @param a 加数
    /// @param b 被加数
    /// @return  QString 和
    QString add(QString a, QString b);
    QString add(QString a, int b);

    /// @brief 减法
    /// @param a 减数
    /// @param b 被减数
    /// @return  QString 差
    QString sub(QString a, QString b);

    /// @brief 乘法
    /// @param a 乘数
    /// @param b 被乘数
    /// @return  QString 积
    QString mul(QString a, QString b);

    /// @brief 除法
    /// @param a 除数
    /// @param b 被除数
    /// @return  QString 商
    QString div(QString a, QString b);

    /// @brief 比较大小
    /// @param a 比较数
    /// @param b 被比较数
    /// @return  bool    a > b 返回 true，否则返回 false
    bool compare(QString a, QString b);
};