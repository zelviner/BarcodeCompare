#pragma once

#include "ui_loading.h"

#include <qmainwindow.h>

class Loading : public QMainWindow {
    Q_OBJECT

  public:
    Loading(QMainWindow *parent = nullptr);
    ~Loading();

  private:
    void initWindow();

  private:
    Ui_Loading *ui_;
};