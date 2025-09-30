#pragma once

#include "box_info.h"
#include "carton_info.h"
#include "database/box_data/box_data_dao.h"
#include "database/carton_data/carton_data_dao.h"
#include "database/order/order.h"

#include <memory>
#include <qstring>

class Comparison {

  public:
    Comparison(const std::shared_ptr<Order> &order, const std::shared_ptr<BoxDataDao> &box_data_dao, const std::shared_ptr<CartonDataDao> &carton_data_dao);
    ~Comparison();

    /// @brief Compare box barcodes.
    int box(const std::shared_ptr<BoxInfo> &box_info);

    /// @brief Compare carton barcodes.
    int carton(const std::shared_ptr<CartonInfo> &carton_info, int &box_widget_id);

  private:
    /// @brief Remove prefixes and suffixes from card barcodes.
    QString trimCardBarcode(const QString &card_barcode);

    /// @brief Remove prefixes and suffixes from box barcodes.
    QString trimBoxBarcode(const QString &box_barcode);

    /// @brief Remove prefixes and suffixes from carton barcodes.
    QString trimCartonBarcode(const QString &carton_barcode);

    /// @brief Remove prefixes and suffixes from barcodes.
    QString trimBarcode(const QString &barcode, int start, int end);

  private:
    std::shared_ptr<Order>         order_;
    std::shared_ptr<BoxDataDao>    box_data_dao_;
    std::shared_ptr<CartonDataDao> carton_data_dao_;
};