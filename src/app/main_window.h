#pragma once

#include "compare/inner_box.h"
#include "compare/outer_box.h"
#include "data/order.h"
#include "data/user.h"
#include "ui_main_window.h"

#include <QMainWindow>

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

    /// @brief 初始化内盒比对 Tab
    void initInnerBoxTab();

    /// @brief 初始化外箱比对 Tab
    void initOuterBoxTab();

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
    void innerBoxSelectOrder();

    /// @brief 获取内盒结束 ICCID
    void toInnerBoxEndIccid();

    /// @brief 获取首卡 ICCID
    void toCardStartIccid();

    /// @brief 获取尾卡 ICCID
    void toCardEndIccid();

    /// @brief 比对内盒
    void compareInnerBox();

    /// @brief 刷新内盒比对 Tab
    void refreshInnerBoxTab();

    /// @brief 外箱选择订单
    void outerBoxSelectOrder();

    /// @brief 获取外箱结束 ICCID
    void toOuterBoxEndIccid();

    /// @brief 获取目标 ICCID
    void toTargetIccid();

    /// @brief 比对外箱
    void compareOuterBox();

    /// @brief 刷新外箱比对 Tab
    void refreshOuterBoxTab();

    /// @brief 添加订单按钮点击事件
    void addOrderBtnClicked();

    /// @brief 修改订单按钮点击事件
    void updateOrderBtnClicked();

    /// @brief 删除订单按钮点击事件
    void removeOrderBtnClicked();

    /// @brief 清空订单按照点击事件
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

    /// @brief 清空订单按照点击事件
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
    OuterBox      *outer_box_;

    QString outbox_start_iccid_;
    QString outbox_end_iccid_;
    int     inbox_row_index_;
    int     outbox_row_index_;
};