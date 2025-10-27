#pragma once

#include "database/dao/card_data/card_data_dao.h"
#include "database/myorm/database.h"

#include <memory>
#include <vector>

class CardDataMysqlDao : public CardDataDao {
  public:
    CardDataMysqlDao(const std::string &order_name);
    ~CardDataMysqlDao();

    /// @brief Batch insert new box datas into the database.
    bool batchAdd(const std::vector<std::shared_ptr<CardData>> &card_datas) override;

    /// @brief Get all card datas from the database.
    std::vector<std::shared_ptr<CardData>> all(const int &status = -1) override;

    /// @brief Set the status of the card data to 1 when it is scanned.
    bool scanned(const std::string &iccid_barcode) override;

    /// @brief Get card data from the database by iccid barcode.
    std::shared_ptr<CardData> get(const std::string &iccid_barcode) override;

    /// @brief Update card data in the database.
    bool update(const int &id, std::shared_ptr<CardData> &card_data) override;

    /// @brief Clear all data in the database.
    bool clear() override;

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<zel::myorm::Database> db_;
    std::string                           order_name_;
};