#pragma once

#include <QString>
#include <QStringList>
#include <QVector>

class User {

  public:
    // 枚举
    enum Role { SuperAdmin, Admin, OrdinaryUser };
    static const char *role_[3];

    /// @brief 用户信息结构体
    struct UserInfo {
        QString name;
        QString password;
        Role    role;
    };

    typedef QVector<User::UserInfo> UserVector;

  public:
    User(QString filename = "data/user.json");
    ~User();

    /// @brief 打印用户列表
    void print() const;

    /// @brief 获取用户列表
    const UserVector &users() const;

    /// @brief 获取用户名列表
    const QStringList &nameList();

    /// @brief 设置当前用户
    /// @param current_user 当前用户
    void currentUser(const UserInfo &current_user);

    /// @brief 获取当前用户
    const UserInfo &currentUser() const;

    /// @brief 登录
    /// @param input_name  输入的用户名
    /// @param input_password  输入的用户名
    bool login(QString input_name, QString input_password);

    /// @brief 添加用户
    /// @param user_info 用户信息结构体
    bool add(const UserInfo &user_info);

    /// @brief 删除用户
    /// @param user_name 用户名
    bool remove(const QString &user_name);

    /// @brief 清空用户
    bool clear();

    /// @brief 更新用户
    bool update(const QString user_name, const UserInfo &user_info);

  private:
    /// @brief 初始化数据文件
    void init();

    /// @brief 加载数据文件
    bool load();

    /// @brief 保存数据文件
    bool save();

  private:
    QString     filename_;
    UserVector  users_;
    QStringList name_list_;
    UserInfo    current_user_;
};