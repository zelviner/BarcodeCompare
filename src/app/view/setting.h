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
    void init_window();

    /// @brief 初始化UI
    void init_ui();

    /// @brief 初始化信号槽
    void init_signals_slots();

  private:
    Ui_Setting                       *ui_;
    std::shared_ptr<SQLite::Database> db_;
    std::shared_ptr<UserDao>          user_dao_;
};
