#pragma once

#include "compare/carton.h"
#include "data/order.h"
#include "data/user.h"
#include "ui_main_window.h"

#include <qmainwindow>
#include <qtranslator>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(User *user, QMainWindow *parent = nullptr);
    ~MainWindow();

  private:
    /// @brief 初始化窗口
    void initWindow();

    /// @brief 初始化 UI
    void initUi();

    /// @brief 切换中文动作触发
    void chineseActionTriggered();

    /// @brief 切换英文动作触发
    void englishActionTriggered();

    /// @brief 切换语言
    void switchLanguage(const QString &language_file);

    /// @brief 初始化内盒比对 Tab
    void initBoxTab();

    /// @brief 初始化外箱比对 Tab
    void initCartonTab();

    /// @brief 初始化订单管理 Tab
    void initOrderTab();

    /// @brief 初始化用户管理 Tab
    void initUserTab();

    /// @brief 初始化表格
    void initTable(QTableWidget *table, QStringList header, int col_count);

    /// @brief 初始化信号槽
    void initSignalSlot();

    /// @brief 切换用户按钮点击事件
    void switchUserBtnClicked();

    /// @brief 内盒选择订单
    void boxSelectOrder();

    /// @brief 清空内盒已扫码数量按钮点击事件
    void clearBoxScannedNumBtnClicked();

    /// @brief 获取内盒结束条码
    void toBoxEndBarcode();

    /// @brief 获取首卡条码
    void toCardStartBarcode();

    /// @brief 获取尾卡条码
    void toCardEndBarcode();

    /// @brief 比对内盒
    void compareBox();

    /// @brief 刷新内盒比对 Tab
    void refreshBoxTab();

    /// @brief 外箱选择订单
    void cartonSelectOrder();

    /// @brief 获取外箱结束条码
    void toCartonEndBarcode();

    /// @brief 获取目标条码
    void toTargetBarcode();

    /// @brief 比对外箱
    void compareCarton();

    /// @brief 刷新外箱比对 Tab
    void refreshCartonTab();

    /// @brief 复位按钮点击事件
    void cartonResetBtnClicked();

    /// @brief 添加订单按钮点击事件
    void addOrderBtnClicked();

    /// @brief 修改订单按钮点击事件
    void updateOrderBtnClicked();

    /// @brief 删除订单按钮点击事件
    void removeOrderBtnClicked();

    /// @brief 清空订单按钮点击事件
    void clearOrderBtnClicked();

    /// @brief 显示选中订单
    void showSelectedOrder();

    /// @brief 刷新订单管理 tab
    void refreshOrderTab();

    /// @brief 添加用户按钮点击事件
    void addUserBtnClicked();

    /// @brief 修改用户按钮点击事件
    void updateUserBtnClicked();

    /// @brief 删除用户按钮点击事件
    void removeUserBtnClicked();

    /// @brief 清空用户按钮点击事件
    void clearUserBtnClicked();

    /// @brief 显示选中用户
    void showSelectedUser();

    ////  @brief 刷新用户管理 Tab
    void refreshUserTab();

    /// @brief 记录日志
    bool log(const QString &filename, const QString &msg);

  private:
    Ui_MainWindow *ui_;
    User          *user_;
    Order         *order_;
    Carton        *carton_;
    QTranslator    translator_;

    QString carton_start_barcode_;
    QString carton_end_barcode_;
    int     box_row_index_;
    int     carton_row_index_;
};