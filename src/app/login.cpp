#include "login.h"

#include "data/user_dao.h"
#include "main_window.h"

#include <memory>
#include <qcompleter>
#include <qdir>
#include <qfile>
#include <qmessagebox>

Login::Login(QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_Login) {
    ui_->setupUi(this);

    initWindow();

    initDir();

    initDatabase();

    initUI();

    initSignalSlot();
}

Login::~Login() { delete ui_; }

void Login::initWindow() {
    // 设置窗口标题
    setWindowTitle(tr("条码比对系统"));
}

void Login::initDir() {
    QString current_path       = QCoreApplication::applicationDirPath();
    QString data_path          = current_path + "/data";
    QString inner_box_log_path = current_path + "/log/内盒";
    QString outer_box_log_path = current_path + "/log/外箱";

    // 创建 data 和 log 目录
    createFolder(data_path);
    createFolder(inner_box_log_path);
    createFolder(outer_box_log_path);
}

void Login::initDatabase() {
    db_                 = std::make_shared<SQLite::Database>("data/barcode_compare.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto box_data_db    = std::make_shared<SQLite::Database>("data/box_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto carton_data_db = std::make_shared<SQLite::Database>("data/carton_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto box_log_db     = std::make_shared<SQLite::Database>("data/box_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto carton_log_db  = std::make_shared<SQLite::Database>("data/carton_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    db_->exec("ATTACH DATABASE 'data/box_data.db' AS box_data;");
    db_->exec("ATTACH DATABASE 'data/carton_data.db' AS carton_data;");
    db_->exec("ATTACH DATABASE 'data/box_log.db' AS box_log;");
    db_->exec("ATTACH DATABASE 'data/carton_log.db' AS carton_log;");

    user_dao_ = std::make_shared<UserDao>(db_);
}

void Login::initUI() {
    // 加载用户信息
    loadUserInfo();

    ui_->user_combo_box->setEditable(true);
    ui_->user_combo_box->lineEdit()->setPlaceholderText(tr("请输入用户名"));
    ui_->user_combo_box->lineEdit()->setAlignment(Qt::AlignCenter);

    // 下拉框模糊匹配
    QStringList user_list;
    for (auto &user : user_dao_->all()) {
        user_list.append(QString::fromStdString(user->name));
    }

    QCompleter *completer = new QCompleter(user_list, this);
    completer->setFilterMode(Qt::MatchContains);
    ui_->user_combo_box->setCompleter(completer);
}

void Login::initSignalSlot() {

    connect(ui_->user_combo_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() { ui_->password_edit->setFocus(); });

    // 回车键
    connect(ui_->password_edit, &QLineEdit::returnPressed, this, &Login::loginBtnClicked);

    // 登录按钮
    connect(ui_->login_btn, &QPushButton::clicked, this, &Login::loginBtnClicked);

    // 退出按钮
    connect(ui_->exit_btn, &QPushButton::clicked, this, &Login::exitBtnClicked);
}

void Login::loadUserInfo() {
    for (auto &user : user_dao_->all()) {
        ui_->user_combo_box->addItem(QString::fromStdString(user->name));
    }

    ui_->user_combo_box->setCurrentIndex(-1);
}

void Login::loginBtnClicked() {

    // 获取用户名和密码
    std::string entered_name     = ui_->user_combo_box->currentText().toStdString();
    std::string entered_password = ui_->password_edit->text().toStdString();

    // 登录
    if (user_dao_->login(entered_name, entered_password)) {
        // 登录成功, 隐藏登录界面
        this->hide();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow(db_, user_dao_);
        mainWindow->show();
    } else {
        // 登录失败
        QMessageBox::warning(this, tr("登录失败"), tr("用户名或密码错误"));
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