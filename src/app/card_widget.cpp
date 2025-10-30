#include "card_widget.h"
#include <qboxlayout.h>
#include <qpixmap.h>

CardWidget::CardWidget(const std::shared_ptr<CardData> &card_data, QWidget *parent)
    : QWidget(parent)
    , card_data_(card_data) {
    // 图片 label
    label_ = new QLabel(this);
    label_->setAlignment(Qt::AlignCenter);
    label_->setFixedSize(16, 16);
    label_->setScaledContents(true); // 自动缩放到固定大小

    // 文字 label
    text_label_ = new QLabel(this);
    text_label_->setAlignment(Qt::AlignCenter);
    text_label_->setText(card_data->card_number.c_str());
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

CardWidget::~CardWidget() {}

void CardWidget::unscanned() { label_->setPixmap(QPixmap(":/image/card_unscanned.png")); }

void CardWidget::pending() { label_->setPixmap(QPixmap(":/image/card_pending.png")); }

void CardWidget::scanned() { label_->setPixmap(QPixmap(":/image/card_scanned.png")); }

const std::shared_ptr<CardData> &CardWidget::cardData() { return card_data_; }
