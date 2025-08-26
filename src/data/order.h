#pragma once

#include <qstring>
#include <qstringlist>
#include <qvector>

class Order {

  public:
    // 枚举
    enum Mode { FirstAndEnd, First };
    static const QVector<QString> mode_;

    struct OrderInfo {
        QString order_name;             // 订单名称
        int     box_count;              // 每盒卡片数量
        int     carton_count;           // 每箱卡片数量
        QString check_format;           // 校验格式
        int     carton_start_check_num; // 外箱开始校验位数
        int     carton_end_check_num;   // 外箱结束校验位数
        int     box_start_check_num;    // 内盒开始校验位数
        int     box_end_check_num;      // 内盒结束校验位数
        int     card_start_check_num;   // 卡片开始校验位数
        int     card_end_check_num;     // 卡片结束校验位数
        int     box_scanned_num;        // 已扫描内盒数量
        int     carton_scanned_num;     // 已扫描外箱数量
        Mode    barcode_mode;           // 条码模式
        QString create_time;            // 创建时间
    };

    typedef QVector<Order::OrderInfo> OrderVector;

  public:
    Order(QString filename = "data/order.json");
    ~Order();

    /// @brief 打印订单列表
    void print() const;

    /// @brief 获取订单列表
    const OrderVector &orders() const;

    /// @brief 获取订单名称列表
    const QStringList &nameList();

    /// @brief 设置当前订单
    /// @param current_order 当前订单
    bool currentOrder(const QString &current_order);

    /// @brief 获取当前订单
    const OrderInfo &currentOrder() const;

    /// @brief 添加订单
    /// @param order_info 订单信息结构体
    bool add(const OrderInfo &order_info);

    /// @brief 删除订单
    /// @param order_name 订单名称
    bool remove(const QString &order_name);

    /// @brief 清空已扫描内盒数量
    bool clearBoxScannedNum();

    /// @brief 清空已扫描外箱数量
    bool clearCartonScannedNum();

    /// @brief 清空订单
    bool clear();

    /// @brief 修改订单
    /// @param order_name 订单名称
    /// @param order_info 新订单信息结构体
    bool update(const QString &order_name, const OrderInfo &order_info);
    bool updateCarton(const QString &order_name, const OrderInfo &order_info);

    bool scanned();
    bool scannedCarton();

  private:
    /// @brief 加载数据文件
    bool load();

    /// @brief 保存数据文件
    bool save();

  private:
    QString     filename_;
    OrderVector orders_;
    QStringList name_list_;
    OrderInfo   current_order_;
};