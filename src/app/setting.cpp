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

    initUI();

    initSignalSlot();
}

Setting::~Setting() { delete ui_; }

void Setting::initWindow() {
    // 设置窗口标题
    setWindowTitle(tr("条码比对系统"));
}

void Setting::initUI() {}

void Setting::initSignalSlot() {}
