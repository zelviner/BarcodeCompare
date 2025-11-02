#pragma once

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGroupBox>
#include <QMimeData>

class OrderGroupBox : public QGroupBox {
    Q_OBJECT
  public:
    explicit OrderGroupBox(QWidget *parent = nullptr)
        : QGroupBox(parent) {
        setAcceptDrops(true);
    }

  signals:
    void fileDropped(const QString &file);

  protected:
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) event->acceptProposedAction();
    }

    void dropEvent(QDropEvent *event) override {
        auto urls = event->mimeData()->urls();
        if (!urls.isEmpty()) emit fileDropped(urls.first().toLocalFile());
    }
};
