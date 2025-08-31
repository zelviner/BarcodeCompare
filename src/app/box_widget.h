#pragma once

#include <qlabel.h>
#include <qlabel>
#include <qwidget>

// 内盒容器
class BoxWidget : public QWidget {
    Q_OBJECT
  public:
    BoxWidget(const int &id, QWidget *parent = nullptr);
    ~BoxWidget();

    void unscanned();

    void scanned();

    int id();

  private:
    int     id_;
    QLabel *label_;
    QLabel *text_label_;
};