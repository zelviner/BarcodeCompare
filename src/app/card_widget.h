#pragma once

#include <qlabel.h>
#include <qlabel>
#include <qwidget>

// 卡片容器
class CardWidget : public QWidget {
    Q_OBJECT
  public:
    CardWidget(const int &id, const QString &card_number, QWidget *parent = nullptr);
    ~CardWidget();

    void unscanned();

    void pending();

    void scanned();

    int id();

    QString cardNumber();

  private:
    int     id_;
    QString card_number_;
    QLabel *label_;
    QLabel *text_label_;
};