#pragma once

#include <qlabel.h>
#include <qlabel>
#include <qwidget>

// 内盒容器
class BoxWidget : public QWidget {
    Q_OBJECT
  public:
    BoxWidget(const int &id, const QString &box_number, QWidget *parent = nullptr);
    ~BoxWidget();

    void unscanned();

    void pending();

    void scanned();

    int id();

    QString boxNumber();

  private:
    int     id_;
    QString box_number_;
    QLabel *label_;
    QLabel *text_label_;
};