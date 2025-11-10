#pragma once

#include "database/card_data/card_data.h"

#include <qlabel.h>
#include <qlabel>
#include <qwidget>

// 卡片容器
class CardWidget : public QWidget {
    Q_OBJECT
  public:
    CardWidget(const std::shared_ptr<CardData> &card_data, QWidget *parent = nullptr);
    ~CardWidget();

    void unscanned();

    void pending();

    void scanned();

    const std::shared_ptr<CardData> &cardData();

  private:
    std::shared_ptr<CardData> card_data_;
    QLabel                   *label_;
    QLabel                   *text_label_;
};