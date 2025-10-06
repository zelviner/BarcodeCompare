#pragma once

#include "database/dao/user/user_dao.h"
#include "database/myorm/database.h"

#include <memory>
#include <qmainwindow>
#include <ui_login.h>

class Login : public QMainWindow {
    Q_OBJECT

  public:
    Login(QMainWindow *parent = nullptr);
    ~Login();

    /// @brief 登录按钮点击事件
    void loginBtnClicked();

    /// @brief 退出按钮点击事件
    void exitBtnClicked();

  private:
    /// @brief 初始化窗口
    void initWindow();

    /// @brief 初始化文件夹
    void initDir();

    /// @brief 初始化配置文件
    void initConfig();

    /// @brief 初始化日志器
    void initLogger();

    /// @brief 初始化 SQLite 数据库
    bool initSQLite();

    /// @brief 初始化 MySQL 数据库
    bool initMySQL();

    /// @brief 初始化UI
    void initUI();

    /// @brief 初始化信号槽
    void initSignalSlot();

    /// @brief 创建文件夹
    /// @param folder_path  文件夹路径
    QString createFolder(const QString &folder_path);

  private:
    Ui_Login                             *ui_;
    std::shared_ptr<SQLite::Database>     sqlite_db_;
    std::shared_ptr<zel::myorm::Database> mysql_db_;
    std::shared_ptr<UserDao>              user_dao_;
};
