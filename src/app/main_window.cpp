#include "main_window.h"
#include "login.h"

#include <QCompleter>
#include <QDateTime>
#include <QDebug>
#include <QKeyEvent> //用于键盘事件
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(User *user, QMainWindow *parent)
    : QMainWindow(parent)
    , ui_(new Ui_MainWindow)
    , order_(new Order)
    , user_(user)
    , carton_(nullptr)
    , carton_start_iccid_("")
    , carton_end_iccid_("") {
    ui_->setupUi(this);

    initWindow();

    initUi();

    initSignalSlot();

    refreshBoxTab();
}

MainWindow::~MainWindow() {
    delete ui_;
    if (order_ != nullptr) {
        delete order_;
        order_ = nullptr;
    }
    if (user_ != nullptr) {
        delete user_;
        user_ = nullptr;
    }
}

void MainWindow::initWindow() {
    // 设置窗口标题
    setWindowTitle("条码比对系统");
}

void MainWindow::initUi() {

    // 初始化内盒比对 tab
    initBoxTab();

    // 初始化外箱比对 tab
    initCartonTab();

    // 初始化订单管理 tab
    initOrderTab();

    // 初始化用户管理 tab
    initUserTab();
}

void MainWindow::initBoxTab() {
    ui_->current_user_label->setText(user_->currentUser().name);

    ui_->box_order_name_combo->setEditable(true);
    ui_->box_order_name_combo->lineEdit()->setPlaceholderText("请输入订单号");
    ui_->box_order_name_combo->lineEdit()->setAlignment(Qt::AlignCenter);

    // 下拉框模糊匹配
    QCompleter *completer = new QCompleter(order_->nameList(), this);
    completer->setFilterMode(Qt::MatchContains);
    ui_->box_order_name_combo->setCompleter(completer);

    QStringList box_header = {"内盒起始ICCID", "内盒结束ICCID", "首卡ICCID", "尾卡ICCID"};
    initTable(ui_->box_table, box_header, box_header.size());
}

void MainWindow::initCartonTab() {
    ui_->current_user_label->setText(user_->currentUser().name);

    ui_->carton_order_name_combo->setEditable(true);
    ui_->carton_order_name_combo->lineEdit()->setPlaceholderText("请输入订单号");
    ui_->carton_order_name_combo->lineEdit()->setAlignment(Qt::AlignCenter);

    // 下拉框模糊匹配
    QCompleter *completer = new QCompleter(order_->nameList(), this);
    completer->setFilterMode(Qt::MatchContains);
    ui_->carton_order_name_combo->setCompleter(completer);

    QStringList carton_header = {"外箱起始ICCID", "外箱结束ICCID", "内盒起始或结束ICCID"};
    initTable(ui_->carton_table, carton_header, carton_header.size());
}

void MainWindow::initOrderTab() {
    QStringList order_header = {"订单号", "校验格式", "每盒卡数量", "每箱卡数量", "创建时间"};
    initTable(ui_->order_table, order_header, order_header.size());
}

void MainWindow::initUserTab() {
    QStringList user_header = {"用户名", "权限"};
    initTable(ui_->user_table, user_header, user_header.size());
    // 设置权限下拉框内容
    for (int i = 0; i < 3; i++) {
        ui_->selected_combo_box->addItem(User::role_[i]);
    }
}

void MainWindow::initTable(QTableWidget *table, QStringList header, int col_count) {
    table->setRowCount(0);            // 设置行数
    table->setColumnCount(col_count); // 设置列数
    table->setHorizontalHeaderLabels(header);

    // 设置表格样式
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 禁止编辑
    table->setSelectionBehavior(QAbstractItemView::SelectRows);  // 选中整行
    table->setSelectionMode(QAbstractItemView::SingleSelection); // 只能选中一行
    table->setAlternatingRowColors(true);                        // 隔行变色

    // 设置表头字体
    QFont header_font;
    header_font.setPointSize(12);
    header_font.setBold(true);
    table->horizontalHeader()->setFont(header_font);
    table->verticalHeader()->setFont(header_font);

    // 设置表头内容颜色
    table->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    table->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    // 设置表格字体
    QFont font;
    font.setPointSize(12);
    table->setFont(font);

    // 设置自动调整列宽,并可以通过拖动表头来调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 设置表格内容居中
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    // 设置表格内容颜色
    table->setStyleSheet("selection-background-color:lightblue;"); // 选中行背景色
    table->setStyleSheet("background-color:transparent;");         // 背景透明
    table->setStyleSheet("alternate-background-color:lightgray;"); // 隔行变色
    table->setStyleSheet("color:black;");                          // 字体颜色

    // 设置表格边框
    table->setFrameShape(QFrame::NoFrame);                 // 无边框
    table->setShowGrid(false);                             // 不显示网格线
    table->setStyleSheet("border:none;");                  // 边框颜色
    table->setStyleSheet("gridline-color:rgba(0,0,0,0);"); // 网格线颜色
}

void MainWindow::initSignalSlot() {
    // 切换用户按钮
    connect(ui_->switch_user_btn, &QPushButton::clicked, this, &MainWindow::switchUserBtnClicked);

    // 切换 tab
    connect(ui_->tabWidget, &QTabWidget::currentChanged, [=](int index) {
        switch (index) {

        // 内盒比对
        case 0:
            refreshBoxTab();
            break;

        // 外箱比对
        case 1:
            refreshCartonTab();
            break;

        // 订单管理
        case 2:
            refreshOrderTab();
            break;

        // 用户管理
        case 3:
            refreshUserTab();
            break;

        default:
            break;
        }
    });

    // 内盒比对 Tab
    connect(ui_->box_order_name_combo, &QComboBox::currentTextChanged, this, &MainWindow::boxSelectOrder);
    connect(ui_->box_start_line, &QLineEdit::returnPressed, this, &MainWindow::toBoxEndIccid);
    connect(ui_->box_end_line, &QLineEdit::returnPressed, this, &MainWindow::toCardStartIccid);
    connect(ui_->card_start_line, &QLineEdit::returnPressed, this, &MainWindow::toCardEndIccid);
    connect(ui_->card_end_line, &QLineEdit::returnPressed, this, &MainWindow::compareBox);

    // 外箱比对 Tab
    connect(ui_->carton_order_name_combo, &QComboBox::currentTextChanged, this, &MainWindow::cartonSelectOrder);
    connect(ui_->carton_start_line, &QLineEdit::returnPressed, this, &MainWindow::toCartonEndIccid);
    connect(ui_->carton_end_line, &QLineEdit::returnPressed, this, &MainWindow::toTargetIccid);
    connect(ui_->target_line, &QLineEdit::returnPressed, this, &MainWindow::compareCarton);

    // 订单配置 tab
    connect(ui_->order_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::showSelectedOrder);
    connect(ui_->add_order_btn, &QPushButton::clicked, this, &MainWindow::addOrderBtnClicked);
    connect(ui_->update_order_btn, &QPushButton::clicked, this, &MainWindow::updateOrderBtnClicked);
    connect(ui_->remove_order_btn, &QPushButton::clicked, this, &MainWindow::removeOrderBtnClicked);
    connect(ui_->clear_order_btn, &QPushButton::clicked, this, &MainWindow::clearOrderBtnClicked);

    // 用户配置 tab
    connect(ui_->user_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::showSelectedUser);
    connect(ui_->update_user_btn, &QPushButton::clicked, this, &MainWindow::updateUserBtnClicked);
    connect(ui_->add_user_btn, &QPushButton::clicked, this, &MainWindow::addUserBtnClicked);
    connect(ui_->remove_user_btn, &QPushButton::clicked, this, &MainWindow::removeUserBtnClicked);
    connect(ui_->clear_user_btn, &QPushButton::clicked, this, &MainWindow::clearUserBtnClicked);
}

void MainWindow::switchUserBtnClicked() {
    // 关闭键盘监听
    this->releaseKeyboard();

    Login *login = new Login();
    login->show();
    this->close();
}

void MainWindow::boxSelectOrder() {
    ui_->box_check_format_label->clear();
    ui_->box_scanned_num_label->clear();

    // 获取订单号
    QString order_name = ui_->box_order_name_combo->currentText();
    if (!order_->currentOrder(order_name)) {
        return;
    }

    // 获取订单信息
    Order::OrderInfo order_info = order_->currentOrder();

    // 显示订单信息
    ui_->box_check_format_label->setText(order_info.check_format);
    ui_->box_scanned_num_label->setText(QString::number(order_info.box_scanned_num) + " 盒");

    // 聚焦到起始ICCID输入框
    ui_->box_start_line->setFocus();
}

void MainWindow::toBoxEndIccid() {
    if (ui_->box_check_format_label->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择订单");
        ui_->box_start_line->clear();
        return;
    }

    ui_->box_end_line->setFocus();
}

void MainWindow::toCardStartIccid() { ui_->card_start_line->setFocus(); }

void MainWindow::toCardEndIccid() { ui_->card_end_line->setFocus(); }

void MainWindow::compareBox() {
    Box::BoxInfo inner_box_info = {ui_->box_start_line->text(),
                                   ui_->box_end_line->text(),
                                   ui_->card_start_line->text(),
                                   ui_->card_end_line->text(),
                                   order_->currentOrder().box_start_check_num,
                                   order_->currentOrder().box_end_check_num,
                                   order_->currentOrder().card_start_check_num,
                                   order_->currentOrder().card_end_check_num};

    Box box(&inner_box_info);

    QString error, log_msg;
    if (box.compare(error)) {
        log_msg.sprintf("用户[%s] 内盒起始ICCID[%s] 内盒结束ICCID[%s] 首卡ICCID[%s] 尾卡ICCID[%s], 扫描成功", user_->currentUser().name.toStdString().c_str(),
                        inner_box_info.start_iccid.toStdString().c_str(), inner_box_info.end_iccid.toStdString().c_str(),
                        inner_box_info.start_card_iccid.toStdString().c_str(), inner_box_info.end_card_iccid.toStdString().c_str());

        order_->scanned();
        ui_->box_scanned_num_label->setText(QString::number(order_->currentOrder().box_scanned_num) + " 盒");

        // 设置表格内容
        ui_->box_table->setRowCount(box_row_index_ + 2);
        ui_->box_table->setItem(box_row_index_, 0, new QTableWidgetItem(inner_box_info.start_iccid));
        ui_->box_table->setItem(box_row_index_, 1, new QTableWidgetItem(inner_box_info.end_iccid));
        ui_->box_table->setItem(box_row_index_, 2, new QTableWidgetItem(inner_box_info.start_card_iccid));
        ui_->box_table->setItem(box_row_index_, 3, new QTableWidgetItem(inner_box_info.end_card_iccid));

        // 设置内容居中
        ui_->box_table->item(box_row_index_, 0)->setTextAlignment(Qt::AlignCenter);
        ui_->box_table->item(box_row_index_, 1)->setTextAlignment(Qt::AlignCenter);
        ui_->box_table->item(box_row_index_, 2)->setTextAlignment(Qt::AlignCenter);
        ui_->box_table->item(box_row_index_, 3)->setTextAlignment(Qt::AlignCenter);

        box_row_index_++;
    } else {
        log_msg.sprintf("用户[%s] 内盒起始ICCID[%s] 内盒结束ICCID[%s] 首卡ICCID[%s] 尾卡ICCID[%s], 扫描失败，失败原因[%s]",
                        user_->currentUser().name.toStdString().c_str(), inner_box_info.start_iccid.toStdString().c_str(),
                        inner_box_info.end_iccid.toStdString().c_str(), inner_box_info.start_card_iccid.toStdString().c_str(),
                        inner_box_info.end_card_iccid.toStdString().c_str(), error.toStdString().c_str());

        QMessageBox::warning(this, "提示", "比对失败: " + error);
    }

    if (!log("内盒/" + order_->currentOrder().order_name, log_msg)) {
        printf("log write error\n");
    }

    ui_->box_start_line->clear();
    ui_->box_end_line->clear();
    ui_->card_start_line->clear();
    ui_->card_end_line->clear();

    ui_->box_start_line->setFocus();
}

void MainWindow::refreshBoxTab() {
    ui_->box_order_name_combo->clear();
    ui_->box_check_format_label->clear();
    ui_->box_scanned_num_label->clear();
    box_row_index_ = 0;
    ui_->box_table->setRowCount(0);

    // 设置订单号下拉框内容
    for (int i = 0; i < order_->orders().size(); i++) {
        ui_->box_order_name_combo->addItem(order_->orders()[i].order_name);
    }

    // 下框默认不选中
    ui_->box_order_name_combo->setCurrentIndex(-1);

    if (user_->currentUser().role == 0) {
        ui_->box_order_name_combo->setEnabled(false);
        ui_->box_start_line->setEnabled(false);
        ui_->box_end_line->setEnabled(false);
        ui_->card_start_line->setEnabled(false);
        ui_->card_end_line->setEnabled(false);
    }
}

void MainWindow::cartonSelectOrder() {
    ui_->carton_check_format_label->clear();
    ui_->carton_scanned_num_label->clear();

    // 获取订单号
    QString order_name = ui_->carton_order_name_combo->currentText();
    if (!order_->currentOrder(order_name)) {
        return;
    }

    // 获取订单信息
    Order::OrderInfo order_info = order_->currentOrder();

    // 显示订单信息
    ui_->carton_check_format_label->setText(order_info.check_format);
    ui_->carton_scanned_num_label->setText(QString::number(order_info.carton_scanned_num) + " 盒");
    // ui_->carton__label->setText(QString::number(order_info.carton_count) + " 张");

    // 聚焦到起始ICCID输入框
    ui_->carton_start_line->setFocus();
}

void MainWindow::toCartonEndIccid() {
    if (ui_->carton_check_format_label->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择订单");
        ui_->carton_start_line->clear();
        return;
    }

    // 计算箱中的盒数
    int box_count = (ui_->carton_end_line->text().toInt() - ui_->carton_start_line->text().toInt()) / order_->currentOrder().carton_count + 1;
    ui_->carton_scanned_num_label->setText(QString::number(order_->currentOrder().carton_scanned_num) + " 盒");

    ui_->carton_end_line->setFocus();
}

void MainWindow::toTargetIccid() { ui_->target_line->setFocus(); }

void MainWindow::compareCarton() {

    Carton::CartonInfo carton_info;
    carton_info.start_iccid          = ui_->carton_start_line->text();
    carton_info.end_iccid            = ui_->carton_end_line->text();
    carton_info.target_iccid         = ui_->target_line->text();
    carton_info.start_check_num      = order_->currentOrder().carton_start_check_num;
    carton_info.end_check_num        = order_->currentOrder().carton_end_check_num;
    carton_info.card_count           = order_->currentOrder().carton_count;
    carton_info.card_start_check_num = order_->currentOrder().card_start_check_num;
    carton_info.card_end_check_num   = order_->currentOrder().card_end_check_num;

    if (carton_info.start_iccid != carton_start_iccid_ || carton_info.end_iccid != carton_end_iccid_) {
        if (carton_ != nullptr) {
            delete carton_;
            carton_ = nullptr;
        }
        carton_ = new Carton();

        carton_start_iccid_ = carton_info.start_iccid;
        carton_end_iccid_   = carton_info.end_iccid;
    }

    carton_->cartonInfo(&carton_info);
    QString error, log_msg;
    bool    is_end;
    if (carton_->compare(error, is_end)) {
        log_msg.sprintf("用户[%s] 外箱起始ICCID[%s] 外箱结束ICCID[%s] 内盒起始或结束ICCID[%s], 扫描成功", user_->currentUser().name.toStdString().c_str(),
                        carton_info.start_iccid.toStdString().c_str(), carton_info.end_iccid.toStdString().c_str(),
                        carton_info.target_iccid.toStdString().c_str());

        // 设置表格内容
        ui_->carton_table->setRowCount(carton_row_index_ + 2);
        ui_->carton_table->setItem(carton_row_index_, 0, new QTableWidgetItem(carton_info.start_iccid));
        ui_->carton_table->setItem(carton_row_index_, 1, new QTableWidgetItem(carton_info.end_iccid));
        ui_->carton_table->setItem(carton_row_index_, 2, new QTableWidgetItem(carton_info.target_iccid));

        // 设置内容居中
        ui_->carton_table->item(carton_row_index_, 0)->setTextAlignment(Qt::AlignCenter);
        ui_->carton_table->item(carton_row_index_, 1)->setTextAlignment(Qt::AlignCenter);
        ui_->carton_table->item(carton_row_index_, 2)->setTextAlignment(Qt::AlignCenter);

        carton_row_index_++;

    } else {
        log_msg.sprintf("用户[%s] 外箱起始ICCID[%s] 外箱结束ICCID[%s] 内盒起始或结束ICCID[%s], 扫描失败，失败原因[%s]",
                        user_->currentUser().name.toStdString().c_str(), carton_info.start_iccid.toStdString().c_str(),
                        carton_info.end_iccid.toStdString().c_str(), carton_info.target_iccid.toStdString().c_str(), error.toStdString().c_str());

        QMessageBox::warning(this, "提示", "比对失败: " + error);
    }

    if (!log("外箱/" + order_->currentOrder().order_name, log_msg)) {
        printf("log write error\n");
    }

    if (is_end) {

        carton_start_iccid_ = "";
        carton_end_iccid_   = "";

        ui_->carton_start_line->clear();
        ui_->carton_end_line->clear();
        ui_->target_line->clear();

        ui_->carton_start_line->setFocus();
    } else {
        ui_->target_line->clear();
        ui_->target_line->setFocus();
    }
}

void MainWindow::refreshCartonTab() {
    ui_->carton_order_name_combo->clear();
    ui_->carton_check_format_label->clear();
    ui_->carton_scanned_num_label->clear();
    carton_row_index_ = 0;
    ui_->carton_table->setRowCount(0);

    // 设置订单号下拉框内容
    for (int i = 0; i < order_->orders().size(); i++) {
        ui_->carton_order_name_combo->addItem(order_->orders()[i].order_name);
    }

    // 下框默认不选中
    ui_->carton_order_name_combo->setCurrentIndex(-1);

    if (user_->currentUser().role == 0) {
        ui_->carton_order_name_combo->setEnabled(false);
        ui_->carton_start_line->setEnabled(false);
        ui_->carton_end_line->setEnabled(false);
        ui_->target_line->setEnabled(false);
    }
}

void MainWindow::addOrderBtnClicked() {
    // 获取订单信息
    QString order_name             = ui_->order_name_line->text();
    int     box_count              = ui_->box_count_line->text().toInt();
    int     carton_count           = ui_->carton_count_line->text().toInt();
    int     box_start_check_num    = ui_->box_start_spin_box->value();
    int     box_end_check_num      = ui_->box_end_spin_box->value();
    int     carton_start_check_num = ui_->carton_start_spin_box->value();
    int     carton_end_check_num   = ui_->carton_end_spin_box->value();
    int     card_start_check_num   = ui_->card_start_spin_box->value();
    int     card_end_check_num     = ui_->card_end_spin_box->value();
    int     box_scanned_num        = 0;
    int     carton_scanned_num     = 0;

    QString check_format;
    check_format.sprintf("卡片：%d 位 - %d 位\n内盒：%d 位 - %d 位\n外箱：%d 位 - %d 位", card_start_check_num, card_end_check_num, box_start_check_num,
                         box_end_check_num, carton_start_check_num, carton_end_check_num);

    QString          create_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    Order::OrderInfo new_order   = {order_name,           box_count,           carton_count,      check_format,         carton_start_check_num,
                                    carton_end_check_num, box_start_check_num, box_end_check_num, card_start_check_num, card_end_check_num,
                                    box_scanned_num,      carton_scanned_num,  create_time};

    if (order_name == "" || box_end_check_num == 0) {
        QMessageBox::warning(this, "添加失败", "订单号或校验位数不能为空");
        return;
    }

    // 添加订单
    if (order_->add(new_order)) {
        QMessageBox::information(this, "添加成功", "订单添加成功");
        refreshOrderTab();
    } else {
        QMessageBox::warning(this, "添加失败", "订单添加失败");
    }
}

void MainWindow::updateOrderBtnClicked() {
    // 获取选中的行索引
    int index = ui_->order_table->currentRow();

    // 获取订单信息
    QString order_name             = ui_->order_name_line->text();
    int     box_count              = ui_->box_count_line->text().toInt();
    int     carton_count           = ui_->carton_count_line->text().toInt();
    int     carton_start_check_num = ui_->carton_start_spin_box->value();
    int     carton_end_check_num   = ui_->carton_end_spin_box->value();
    int     box_start_check_num    = ui_->box_start_spin_box->value();
    int     box_end_check_num      = ui_->box_end_spin_box->value();
    int     card_start_check_num   = ui_->card_start_spin_box->value();
    int     card_end_check_num     = ui_->card_end_spin_box->value();
    int     box_scanned_num        = 0;
    int     carton_scanned_num     = 0;

    QString check_format;
    check_format.sprintf("卡片：%d 位 - %d 位\n内盒：%d 位 - %d 位\n外箱：%d 位 - %d 位", card_start_check_num, card_end_check_num, box_start_check_num,
                         box_end_check_num, carton_start_check_num, carton_end_check_num);
    QString          create_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    Order::OrderInfo new_order   = {order_name,           box_count,           carton_count,      check_format,         carton_start_check_num,
                                    carton_end_check_num, box_start_check_num, box_end_check_num, card_start_check_num, card_end_check_num,
                                    box_scanned_num,      carton_scanned_num,  create_time};

    if (order_name == "" || box_end_check_num == 0 || index == -1) {
        QMessageBox::warning(this, "修改失败", "订单号或校验位数不能为空");
        return;
    }

    // 保存订单信息
    if (order_->update(order_->orders()[index].order_name, new_order)) {
        QMessageBox::information(this, "保存成功", "订单信息保存成功");
        refreshOrderTab();
    } else {
        QMessageBox::warning(this, "保存失败", "订单信息保存失败");
    }
}

void MainWindow::removeOrderBtnClicked() {

    // 获取选中的行索引
    int index = ui_->order_table->currentRow();

    if (index == -1) {
        QMessageBox::warning(this, "删除失败", "请选中要删除的订单");
        return;
    }

    // 弹窗确认
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "删除订单", "确认删除 [" + order_->orders()[index].order_name + "] 吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        refreshOrderTab();
        return;
    }

    // 删除订单
    if (order_->remove(order_->orders()[index].order_name)) {
        QMessageBox::information(this, "删除成功", "订单删除成功");
        refreshOrderTab();
    } else {
        QMessageBox::warning(this, "删除失败", "订单删除失败");
    }
}

void MainWindow::clearOrderBtnClicked() {
    // 弹窗确认
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "清空订单", "确认清空所有订单吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        refreshOrderTab();
        return;
    }

    if (order_->clear()) {
        QMessageBox::information(this, "清空成功", "订单清空成功");
        refreshOrderTab();
    } else {
        QMessageBox::warning(this, "清空失败", "订单清空失败");
    }
}

void MainWindow::showSelectedOrder() {
    // 获取选中的行索引
    int index = ui_->order_table->currentRow();

    ui_->order_name_line->setText(order_->orders()[index].order_name);
    ui_->box_start_spin_box->setValue(order_->orders()[index].box_start_check_num);
    ui_->box_end_spin_box->setValue(order_->orders()[index].box_end_check_num);
    ui_->box_count_line->setText(QString::number(order_->orders()[index].box_count));
    ui_->carton_count_line->setText(QString::number(order_->orders()[index].carton_count));
    ui_->card_start_spin_box->setValue(order_->orders()[index].card_start_check_num);
    ui_->card_end_spin_box->setValue(order_->orders()[index].card_end_check_num);
}

void MainWindow::refreshOrderTab() {
    // 获取订单管理 tableWidget
    ui_->order_table->setRowCount(order_->orders().size()); // 设置行数
    QStringList header = {"订单号", "校验格式", "每盒卡数量", "每箱卡数量", "创建时间"};
    ui_->order_table->setColumnCount(header.size()); // 设置列数

    ui_->order_table->setHorizontalHeaderLabels(header);

    // 设置表格内容
    for (int i = 0; i < order_->orders().size(); i++) {
        ui_->order_table->setItem(i, 0, new QTableWidgetItem(order_->orders()[i].order_name));
        ui_->order_table->setItem(i, 1, new QTableWidgetItem(order_->orders()[i].check_format));
        ui_->order_table->setItem(i, 2, new QTableWidgetItem(QString::number(order_->orders()[i].box_count)));
        ui_->order_table->setItem(i, 3, new QTableWidgetItem(QString::number(order_->orders()[i].carton_count)));
        ui_->order_table->setItem(i, 4, new QTableWidgetItem(order_->orders()[i].create_time));

        // 设置内容居中
        ui_->order_table->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui_->order_table->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        ui_->order_table->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        ui_->order_table->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        ui_->order_table->item(i, 4)->setTextAlignment(Qt::AlignCenter);
    }

    // 根据内容调整列高
    ui_->order_table->resizeRowsToContents();

    ui_->order_name_line->clear();
    ui_->box_count_line->clear();
    ui_->box_start_spin_box->setValue(1);
    ui_->box_end_spin_box->setValue(19);

    if (user_->currentUser().role == 1) {
        ui_->clear_order_btn->setEnabled(false);
    } else if (user_->currentUser().role == 2) {
        ui_->add_order_btn->setEnabled(false);
        ui_->remove_order_btn->setEnabled(false);
        ui_->update_order_btn->setEnabled(false);
        ui_->clear_order_btn->setEnabled(false);

        ui_->order_name_line->setEnabled(false);
        ui_->box_start_spin_box->setEnabled(false);
        ui_->box_end_spin_box->setEnabled(false);
        ui_->card_start_spin_box->setEnabled(false);
        ui_->card_end_spin_box->setEnabled(false);
        ui_->box_count_line->setEnabled(false);
    }
}

void MainWindow::addUserBtnClicked() {
    // 获取用户信息
    QString        name     = ui_->selected_user_edit->text();
    QString        password = ui_->selected_password_edit->text();
    int            role     = ui_->selected_combo_box->currentIndex();
    User::UserInfo new_user = {name, password, (User::Role) role};

    if (name == "" || password == "") {
        QMessageBox::warning(this, "添加失败", "用户名或密码不能为空");
        return;
    }

    // 添加用户
    if (user_->add(new_user)) {
        QMessageBox::information(this, "添加成功", "用户添加成功");
        refreshUserTab();
    } else {
        QMessageBox::warning(this, "添加失败", "用户添加失败");
    }
}

void MainWindow::updateUserBtnClicked() {
    // 获取选中的行索引
    int index = ui_->user_table->currentRow();

    // 获取用户信息
    QString        name     = ui_->selected_user_edit->text();
    QString        password = ui_->selected_password_edit->text();
    int            role     = ui_->selected_combo_box->currentIndex();
    User::UserInfo new_user = {name, password, (User::Role) role};

    if (name == "" || password == "" || index == -1) {
        QMessageBox::warning(this, "修改失败", "用户名或密码不能为空");
        return;
    }

    // 保存用户信息
    if (user_->update(user_->users()[index].name, new_user)) {
        QMessageBox::information(this, "保存成功", "用户信息保存成功");
        refreshUserTab();
    } else {
        QMessageBox::warning(this, "保存失败", "用户信息保存失败");
    }
}

void MainWindow::removeUserBtnClicked() {

    // 获取选中的行索引
    int index = ui_->user_table->currentRow();

    if (index == -1) {
        QMessageBox::warning(this, "删除失败", "请选中要删除的用户");
        return;
    }

    // 弹窗确认
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "删除用户", "确认删除 [" + user_->users()[index].name + "] 吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        refreshUserTab();
        return;
    }

    // 删除用户
    if (user_->remove(user_->users()[index].name)) {
        QMessageBox::information(this, "删除成功", "用户删除成功");
        refreshUserTab();
    } else {
        QMessageBox::warning(this, "删除失败", "用户删除失败");
    }
}

void MainWindow::clearUserBtnClicked() {
    // 弹窗确认
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "清空用户", "确认清空所有用户吗？", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        refreshUserTab();
        return;
    }

    if (user_->clear()) {
        QMessageBox::information(this, "清空成功", "用户清空成功");
        refreshUserTab();
    } else {
        QMessageBox::warning(this, "清空失败", "用户清空失败");
    }
}

void MainWindow::showSelectedUser() {

    // 获取选中的行索引
    int index = ui_->user_table->currentRow();

    ui_->selected_user_edit->setText(user_->users()[index].name);
    ui_->selected_password_edit->setText(user_->users()[index].password);
    ui_->selected_combo_box->setCurrentIndex((int) user_->users()[index].role);
}

void MainWindow::refreshUserTab() {
    // 获取用户管理 tableWidget
    ui_->user_table->setRowCount(user_->users().size()); // 设置行数
    ui_->user_table->setColumnCount(2);                  // 设置列数
    QStringList header = {"用户名", "权限"};
    ui_->user_table->setHorizontalHeaderLabels(header);

    // 设置表格内容
    for (int i = 0; i < user_->users().size(); i++) {
        ui_->user_table->setItem(i, 0, new QTableWidgetItem(user_->users()[i].name));
        int role = (int) user_->users()[i].role;
        ui_->user_table->setItem(i, 1, new QTableWidgetItem(User::role_[role]));

        // 设置内容居中
        ui_->user_table->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        ui_->user_table->item(i, 1)->setTextAlignment(Qt::AlignCenter);
    }

    ui_->selected_user_edit->clear();
    ui_->selected_password_edit->clear();
    ui_->selected_combo_box->setCurrentIndex(-1);

    if (user_->currentUser().role == 1) {
        ui_->clear_user_btn->setEnabled(false);
    } else if (user_->currentUser().role == 2) {
        ui_->add_user_btn->setEnabled(false);
        ui_->remove_user_btn->setEnabled(false);
        ui_->update_user_btn->setEnabled(false);
        ui_->clear_user_btn->setEnabled(false);

        ui_->selected_user_edit->setEnabled(false);
        ui_->selected_password_edit->setEnabled(false);
        ui_->selected_combo_box->setEnabled(false);
    }
}

bool MainWindow::log(const QString &filename, const QString &msg) {

    // 获取当前时间
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // 获取当前程序所在路径
    QString current_path = QCoreApplication::applicationDirPath();

    // 获取日志文件路径
    QString log_path = current_path + "/log/" + filename + ".log";

    // 打开日志文件
    QFile file(log_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        return false;
    }

    // 写入日志
    QTextStream text_stream(&file);
    text_stream << current_date_time << " " << msg << "\r\n";
    file.flush();
    file.close();

    return true;
}