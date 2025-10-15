#include "setting.h"

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

    init_window();

    init_ui();

    init_signals_slots();
}

Setting::~Setting() { delete ui_; }

void Setting::init_window() {
    // 设置窗口标题
    setWindowTitle(tr("条码比对系统"));
}

void Setting::init_ui() {}

void Setting::init_signals_slots() {}
