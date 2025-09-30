#pragma once

#include "database/user/user_dao.h"

#include <memory>
#include <qmainwindow>
#include <ui_setting.h>

class Setting : public QMainWindow {
    Q_OBJECT

  public:
    Setting(QMainWindow *parent = nullptr);
    ~Setting();

  private:
    /// @brief 初始化窗口
    void initWindow();

    /// @brief 初始化UI
    void initUI();

    /// @brief 初始化信号槽
    void initSignalSlot();

  private:
    Ui_Setting                       *ui_;
    std::shared_ptr<SQLite::Database> db_;
    std::shared_ptr<UserDao>          user_dao_;
};
