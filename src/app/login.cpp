#include "login.h"

#include "database/dao/user/user_dao_factory.h"
#include "main_window.h"

#include <memory>
#include <qcompleter>
#include <qdir>
#include <qfile>
#include <qmessagebox>
#include <qsettings>
#include <zel/utility/logger.h>
using namespace zel::utility;

Login::Login(QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_Login)
    , sqlite_db_(nullptr)
    , mysql_db_(nullptr) {
    ui_->setupUi(this);

    initWindow();

    initDir();

    initConfig();

    initLogger();

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

void Login::initConfig() {
    // 查看是否有配置文件
    QFile config_file("config.ini");
    if (config_file.exists()) {
        // // 读取配置文件
        // QSettings settings("config.ini", QSettings::IniFormat);
        // settings.beginGroup("login");
        // ui_->user_combo_box->setCurrentText(settings.value("user").toString());
        // ui_->password_edit->setText(settings.value("password").toString());
        // settings.endGroup();
    } else {
        // 创建配置文件
        QSettings settings("config.ini", QSettings::IniFormat);

        settings.beginGroup("mysql");
        settings.setValue("host", "127.0.0.1");
        settings.setValue("port", "3306");
        settings.setValue("user", "root");
        settings.setValue("password", "root");
        settings.setValue("database", "barcode_compare");
        settings.endGroup();
    }
}

void Login::initLogger() {
    zel::utility::Logger::instance().open("barcode_compare.log");
    zel::utility::Logger::instance().setLevel(zel::utility::Logger::LOG_DEBUG);
}

bool Login::initSQLite() {
    sqlite_db_          = std::make_shared<SQLite::Database>("data/barcode_compare.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto box_data_db    = std::make_shared<SQLite::Database>("data/box_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto carton_data_db = std::make_shared<SQLite::Database>("data/carton_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto box_log_db     = std::make_shared<SQLite::Database>("data/box_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    auto carton_log_db  = std::make_shared<SQLite::Database>("data/carton_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    sqlite_db_->exec("ATTACH DATABASE 'data/box_data.db' AS box_data;");
    sqlite_db_->exec("ATTACH DATABASE 'data/carton_data.db' AS carton_data;");
    sqlite_db_->exec("ATTACH DATABASE 'data/box_log.db' AS box_log;");
    sqlite_db_->exec("ATTACH DATABASE 'data/carton_log.db' AS carton_log;");

    user_dao_ = UserDaoFactory::create(sqlite_db_, mysql_db_);

    return true;
}

bool Login::initMySQL() {
    // 读取配置文件
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("mysql");
    std::string host     = settings.value("host").toString().toStdString();
    int         port     = settings.value("port").toInt();
    std::string user     = settings.value("user").toString().toStdString();
    std::string password = settings.value("password").toString().toStdString();
    std::string database = settings.value("database").toString().toStdString();
    settings.endGroup();

    mysql_db_ = std::make_shared<zel::myorm::Database>();
    if (!mysql_db_->connect(host, port, user, password, database)) {
        return false;
    }

    // 创建数据库
    std::string create_box_data_sql    = "CREATE DATABASE IF NOT EXISTS box_data DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";
    std::string create_carton_data_sql = "CREATE DATABASE IF NOT EXISTS carton_data DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;";
    mysql_db_->execute(create_box_data_sql);
    mysql_db_->execute(create_carton_data_sql);

    user_dao_ = UserDaoFactory::create(sqlite_db_, mysql_db_);
    return true;
}

void Login::initUI() {
    // ui_->user_combo_box->setEditable(true);
    // ui_->user_combo_box->lineEdit()->setPlaceholderText(tr("请输入用户名"));
    // ui_->user_combo_box->lineEdit()->setAlignment(Qt::AlignCenter);

    // // 下拉框模糊匹配
    // QStringList user_list;
    // for (auto &user : user_dao_->all()) {
    //     user_list.append(QString::fromStdString(user->name));
    // }

    // QCompleter *completer = new QCompleter(user_list, this);
    // completer->setFilterMode(Qt::MatchContains);
    // ui_->user_combo_box->setCompleter(completer);
}

void Login::initSignalSlot() {

    // connect(ui_->user_combo_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() { ui_->password_edit->setFocus(); });

    // 回车键
    connect(ui_->password_edit, &QLineEdit::returnPressed, this, &Login::loginBtnClicked);

    // 登录按钮
    connect(ui_->login_btn, &QPushButton::clicked, this, &Login::loginBtnClicked);

    // 退出按钮
    connect(ui_->exit_btn, &QPushButton::clicked, this, &Login::exitBtnClicked);
}

void Login::loginBtnClicked() {

    // 获取用户名和密码
    std::string entered_name     = ui_->username_edit->text().toStdString();
    std::string entered_password = ui_->password_edit->text().toStdString();

    if (ui_->online_radio_button->isChecked()) {
        if (!initMySQL()) {
            QMessageBox::warning(this, tr("连接数据库失败"), tr("请检查服务器数据库配置,或使用离线模式登录"));
            return;
        }
    } else if (ui_->offline_radio_button->isChecked()) {
        initSQLite();
    } else {
        QMessageBox::warning(this, tr("请选择登录模式"), tr("请选择登录模式"));
        return;
    }

    // 登录
    if (user_dao_->login(entered_name, entered_password)) {
        // 登录成功, 隐藏登录界面
        this->hide();

        // 显示主界面
        MainWindow *mainWindow = new MainWindow(sqlite_db_, mysql_db_, user_dao_);
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