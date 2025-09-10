#include "setting.h"

#include "data/user_dao.h"
#include "main_window.h"

#include <memory>
#include <qcompleter>
#include <qdir>
#include <qfile>
#include <qmessagebox>

Setting::Setting(QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_Setting) {
    ui_->setupUi(this);

    initWindow();

    initDatabase();

    initUI();

    initSignalSlot();
}

Setting::~Setting() { delete ui_; }

void Setting::initWindow() {
    // 设置窗口标题
    setWindowTitle(tr("条码比对系统"));
}

void Setting::initUI() {

    // QString current_path       = QCoreApplication::applicationDirPath();
    // QString data_path          = current_path + "/data";
    // QString inner_box_log_path = current_path + "/log/内盒";
    // QString outer_box_log_path = current_path + "/log/外箱";

    // // 创建 data 和 log 目录
    // createFolder(data_path);
    // createFolder(inner_box_log_path);
    // createFolder(outer_box_log_path);

    // // 加载用户信息
    // loadUserInfo();

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

void Setting::initSignalSlot() {

    // connect(ui_->user_combo_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() { ui_->password_edit->setFocus(); });

    // // 回车键
    // connect(ui_->password_edit, &QLineEdit::returnPressed, this, &Setting::loginBtnClicked);

    // // 登录按钮
    // connect(ui_->login_btn, &QPushButton::clicked, this, &Setting::loginBtnClicked);

    // // 退出按钮
    // connect(ui_->exit_btn, &QPushButton::clicked, this, &Setting::exitBtnClicked);
}

void Setting::initDatabase() {
    // // 创建 data 文件夹
    // QDir data_dir("data");
    // if (!data_dir.exists()) {
    //     data_dir.mkpath(".");
    // }

    // db_                 = std::make_shared<SQLite::Database>("data/barcode_compare.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // auto box_data_db    = std::make_shared<SQLite::Database>("data/box_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // auto carton_data_db = std::make_shared<SQLite::Database>("data/carton_data.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // auto box_log_db     = std::make_shared<SQLite::Database>("data/box_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    // auto carton_log_db  = std::make_shared<SQLite::Database>("data/carton_log.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    // db_->exec("ATTACH DATABASE 'data/box_data.db' AS box_data;");
    // db_->exec("ATTACH DATABASE 'data/carton_data.db' AS carton_data;");
    // db_->exec("ATTACH DATABASE 'data/box_log.db' AS box_log;");
    // db_->exec("ATTACH DATABASE 'data/carton_log.db' AS carton_log;");

    // // role_dao_ = std::make_shared<RoleDao>(db_);
    // user_dao_ = std::make_shared<UserDao>(db_);
}

void Setting::loadUserInfo() {
    // for (auto &user : user_dao_->all()) {
    //     ui_->user_combo_box->addItem(QString::fromStdString(user->name));
    // }

    // ui_->user_combo_box->setCurrentIndex(-1);
}
