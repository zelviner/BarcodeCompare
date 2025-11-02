#include "box_widget.h"
#include <qboxlayout.h>
#include <qpixmap.h>

BoxWidget::BoxWidget(const int &id, const QString &box_number, QWidget *parent)
    : QWidget(parent)
    , id_(id)
    , box_number_(box_number) {
    // 图片 label
    label_ = new QLabel(this);
    label_->setAlignment(Qt::AlignCenter);
    label_->setFixedSize(32, 32);
    label_->setScaledContents(true); // 自动缩放到固定大小

    // 文字 label
    text_label_ = new QLabel(this);
    text_label_->setAlignment(Qt::AlignCenter);
    text_label_->setText(box_number);
    text_label_->setStyleSheet("font-size:10px;"); // 字体大小可调整

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label_);
    layout->addWidget(text_label_);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2); // 图片和文字的间距
    setLayout(layout);

    // 默认显示未扫描内盒
    unscanned();
}

BoxWidget::~BoxWidget() {}

void BoxWidget::unscanned() { label_->setPixmap(QPixmap(":/image/box_unscanned.png")); }

void BoxWidget::pending() { label_->setPixmap(QPixmap(":/image/box_pending.png")); }

void BoxWidget::scanned() { label_->setPixmap(QPixmap(":/image/box_scanned.png")); }

int BoxWidget::id() { return id_; }

QString BoxWidget::boxNumber() { return box_number_; }