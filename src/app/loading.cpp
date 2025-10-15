#include "loading.h"

Loading::Loading(QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_Loading) {
    ui_->setupUi(this);

    ui_->loading_progress->setRange(0, 0);

    init_window();
}

Loading::~Loading() { delete ui_; }

void Loading::init_window() {}