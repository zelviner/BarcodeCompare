#include "login.h"

#include "main_window.h"

#include <QCompleter>
#include <QDir>
#include <QFile>
#include <QMessageBox>

Login::Login(QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_Login)
    , user_(new User) {
    ui_->setupUi(this);

    initWindow();

    initUI();

    initSignalSlot();
}

Login::~Login() {
    delete ui_;
    if (user_ != nullptr) {
        delete user_;
        user_ = nullptr;
    }
}

void Login::initWindow() {
    // 设置窗口标题
    setWindowTitle("星汉条码比对系统");
}

void Login::initUI() {

    QString current_path       = QCoreApplication::applicationDirPath();
    QString data_path          = current_path + "/data";
    QString inner_box_log_path = current_path + "/log/内盒";
    QString outer_box_log_path = current_path + "/log/外箱";

    // 创建 data 和 log 目录
    createFolder(data_path);
    createFolder(inner_box_log_path);
    createFolder(outer_box_log_path);

    // 加载用户信息
    loadUserInfo();

    ui_->user_combo_box->setEditable(true);
    ui_->user_combo_box->lineEdit()->setPlaceholderText("请输入用户名");
    ui_->user_combo_box->lineEdit()->setAlignment(Qt::AlignCenter);

    // 下拉框模糊匹配
    QCompleter *completer = new QCompleter(user_->nameList(), this);
    completer->setFilterMode(Qt::MatchContains);
    ui_->user_combo_box->setCompleter(completer);
}

void Login::initSignalSlot() {

    // 回车键
    connect(ui_->password_edit, &QLineEdit::returnPressed, this, &Login::loginBtnClicked);

    // 登录按钮
    connect(ui_->login_btn, &QPushButton::clicked, this, &Login::loginBtnClicked);

    // 退出按钮
    connect(ui_->exit_btn, &QPushButton::clicked, this, &Login::exitBtnClicked);
}

void Login::loadUserInfo() {
    for (auto user : user_->users()) {
        ui_->user_combo_box->addItem(user.name);
    }

    ui_->user_combo_box->setCurrentIndex(-1);
}

void Login::loginBtnClicked() {

    // 获取用户名和密码
    QString input_name     = ui_->user_combo_box->currentText();
    QString input_password = ui_->password_edit->text();

    // 登录
    if (user_->login(input_name, input_password)) {
        // 登录成功, 隐藏登录界面
        this->hide();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow(user_);
        mainWindow->show();
    } else {
        // 登录失败
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void Login::exitBtnClicked() {
    // 退出程序
    exit(0);
}

QString Login::createFolder(const QString &folder_path) {
    QDir dir(folder_path);
    if (dir.exists(folder_path)) {
        return folder_path;
    }

    QString parentDir = createFolder(folder_path.mid(0, folder_path.lastIndexOf('/')));
    QString dirName   = folder_path.mid(folder_path.lastIndexOf('/') + 1);

    QDir parentPath(parentDir);
    if (!dirName.isEmpty()) {
        parentPath.mkpath(dirName);
    }
    return parentDir + "/" + dirName;
}