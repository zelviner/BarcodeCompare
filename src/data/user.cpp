#include "user.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

const char *User::role_[3] = {"超级管理员", "管理员", "普通用户"};

User::User(QString filename)
    : filename_(filename) {
    init();
}

User::~User() {}

void User::print() const {
    for (auto user : users_) {
        printf("name: %s, password: %s, role: %s\n", user.name.toUtf8().data(), user.password.toUtf8().data(),
               User::role_[user.role]);
    }
}

bool User::login(QString input_name, QString input_password) {
    for (auto user : users_) {
        if (user.name == input_name && user.password == input_password) {
            currentUser(user);
            return true;
        }
    }

    return false;
}

const User::UserVector &User::users() const { return users_; }

const QStringList &User::nameList() {
    name_list_.clear();
    for (auto user : users_) {
        name_list_.push_back(user.name);
    }
    return name_list_;
}

void User::currentUser(const UserInfo &current_user) { current_user_ = current_user; }

const User::UserInfo &User::currentUser() const { return current_user_; }

bool User::add(const UserInfo &user_info) {
    users_.push_back(user_info);

    return save();
}

bool User::remove(const QString &user_name) {
    for (auto it = users_.begin(); it != users_.end(); it++) {
        if (it->name == user_name) {
            users_.erase(it);
            break;
        }
    }

    return save();
}

bool User::clear() {
    users_.clear();
    return save();
}

bool User::update(const QString user_name, const UserInfo &user_info) {
    for (auto it = users_.begin(); it != users_.end(); it++) {
        if (it->name == user_name) {
            *it = user_info;
            break;
        }
    }

    return save();
}

void User::init() {
    // 判断文件是否存在
    QFile file(filename_);
    if (!file.exists()) {
        // 创建文件
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.close();

        // 创建超级管理员
        UserInfo user_info;
        user_info.name     = "admin";
        user_info.password = "Xh123456";
        user_info.role     = Role::SuperAdmin;

        users_.push_back(user_info);

        // 保存用户信息
        save();
    } else {
        // 读取用户信息
        load();
    }
}

bool User::load() {

    // 打开JSON文件
    QFile file(filename_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    // 从文件中读取JSON数据
    QByteArray jsonData = file.readAll();
    file.close();

    // 将JSON数据解析为文档
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 获取JSON对象
    QJsonObject jsonObject = jsonDoc.object();

    // 获取用户信息数组
    QJsonArray userInfosArray = jsonObject["user_info"].toArray();

    // 遍历用户信息数组
    for (const QJsonValue &userInfoValue : userInfosArray) {
        QJsonObject userInfoObject = userInfoValue.toObject();

        QString userName = userInfoObject["user_name"].toString();
        QString password = userInfoObject["password"].toString();
        int     role     = userInfoObject["role"].toInt();

        UserInfo user_info;
        user_info.name     = userName;
        user_info.password = password;
        user_info.role     = static_cast<Role>(role);

        users_.push_back(user_info);
    }

    return true;
}

bool User::save() {
    // 创建一个JSON对象
    QJsonObject jsonObject;

    // 创建一个JSON数组
    QJsonArray userArray;

    // 遍历用户列表，将用户信息添加到JSON数组
    for (const UserInfo &user : users_) {
        QJsonObject userInfoObject;
        userInfoObject["user_name"] = user.name;
        userInfoObject["password"]  = user.password;
        userInfoObject["role"]      = static_cast<int>(user.role);
        userArray.append(userInfoObject);
    }

    // 在JSON对象中设置用户信息数组
    jsonObject["user_info"] = userArray;

    // 将JSON对象转换为JSON文档
    QJsonDocument jsonDoc(jsonObject);

    // 打开文件以写入JSON数据
    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    // 将JSON数据写入文件
    file.write(jsonDoc.toJson());
    file.close();

    return true;
}