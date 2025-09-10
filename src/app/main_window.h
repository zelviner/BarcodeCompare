#pragma once

#include "box_widget.h"
#include "data/format_dao.h"
#include "data/mode_dao.h"
#include "data/order_dao.h"
#include "data/role_dao.h"
#include "data/user_dao.h"
#include "loading.h"
#include "ui_main_window.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <qmainwindow>
#include <qtranslator>
#include <vector>

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    MainWindow(const std::shared_ptr<SQLite::Database> &db, const std::shared_ptr<UserDao> &user_dao, QMainWindow *parent = nullptr);
    ~MainWindow();

  private:
    /// @brief 初始化窗口
    void initWindow();

    /// @brief 初始化 UI
    void initUi();

    /// @brief 初始化数据库
    void initDao();

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

    /// @brief 切换用户动作触发
    void switchUserActionTriggered();

    /// @brief 切换中文动作触发
    void chineseActionTriggered();

    /// @brief 切换英文动作触发
    void englishActionTriggered();

    /// @brief 设置标签数据动作触发
    void setTagDataActionTriggered();

    /// @brief 内盒选择订单
    void boxSelectOrder();

    /// @brief 选择内盒数据状态
    void selectBoxDatasStatus();

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

    /// @brief 刷新内盒表格
    void refreshBoxTable(const std::string &order_name, const int &status);

    /// @brief 外箱选择订单
    void cartonSelectOrder();

    /// @brief 显示选中外箱
    void showSelectedCarton();

    /// @brief 选择外箱数据状态
    void selectCartonDatasStatus();

    /// @brief 获取外箱结束条码
    void toCartonEndBarcode();

    /// @brief 获取目标条码
    void toTargetBarcode();

    /// @brief 比对外箱
    void compareCarton();

    /// @brief 刷新外箱比对 Tab
    void refreshCartonTab();

    /// @brief 刷新外箱表格
    void refreshCartonTable(const std::string &order_name, const int &status);

    /// @brief 刷新内盒比对组
    void refreshBoxCompareGroup(const int &cols, const std::string &selected_carton_start_barcode);

    /// @brief 选择内盒文件按钮点击事件
    void selectBoxFileBtnClicked();

    /// @brief 选择外箱文件按钮点击事件
    void selectCartonFileBtnClicked();

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

  public slots:
    void addOrderSuccess();
    void addOrderFailure();

  private:
    /// @brief 切换语言
    void switchLanguage(const QString &language_file);

    /// @brief 记录日志
    bool log(const QString &filename, const QString &msg);

    void scrollToValue(QTableWidget *table, const QString &value, bool selected = true);

    void clearBoxCompareGroupLayout(QLayout *layout);

  private:
    Ui_MainWindow           *ui_;
    QTranslator              translator_;
    std::shared_ptr<Loading> loading_;
    std::vector<BoxWidget *> box_widgets_;

    std::shared_ptr<SQLite::Database> db_;
    std::shared_ptr<RoleDao>          role_dao_;
    std::shared_ptr<UserDao>          user_dao_;
    std::shared_ptr<ModeDao>          mode_dao_;
    std::shared_ptr<OrderDao>         order_dao_;
    std::shared_ptr<FormatDao>        format_dao_;
};