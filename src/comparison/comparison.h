#pragma once

#include "box_info.h"
#include "card_info.h"
#include "carton_info.h"
#include "database/dao/box_data/box_data_dao.h"
#include "database/dao/card_data/card_data_dao.h"
#include "database/dao/carton_data/carton_data_dao.h"
#include "database/dao/order/order.h"

#include <memory>
#include <qstring>

class Comparison {

  public:
    Comparison(const std::shared_ptr<Order> &order, const std::shared_ptr<BoxDataDao> &box_data_dao,
               const std::shared_ptr<CartonDataDao> &carton_data_dao = nullptr, const std::shared_ptr<CardDataDao> &card_data_dao = nullptr);
    ~Comparison();

    /// @brief Compare box barcodes.
    int box(const std::shared_ptr<BoxInfo> &box_info);

    /// @brief Compare carton barcodes.
    int carton(const std::shared_ptr<CartonInfo> &carton_info, int &box_widget_id);

    /// @brief Compare card barcodes.
    int card(const std::shared_ptr<CardInfo> &card_info, int &card_widget_id);

  private:
    /// @brief Remove prefixes and suffixes from card barcodes.
    QString trim_card_barcode(const QString &card_barcode);

    /// @brief Remove prefixes and suffixes from box barcodes.
    QString trim_box_barcode(const QString &box_barcode);

    /// @brief Remove prefixes and suffixes from carton barcodes.
    QString trim_carton_barcode(const QString &carton_barcode);

    /// @brief Remove prefixes and suffixes from barcodes.
    QString trim_barcode(const QString &barcode, int start, int end);

  private:
    std::shared_ptr<Order>         order_;
    std::shared_ptr<BoxDataDao>    box_data_dao_;
    std::shared_ptr<CartonDataDao> carton_data_dao_;
    std::shared_ptr<CardDataDao>   card_data_dao_;
};