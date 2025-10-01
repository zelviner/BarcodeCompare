#pragma once

#include "database/dao/carton_data/carton_data_dao.h"
#include "database/myorm/database.h"

#include <memory>
#include <vector>

class CartonDataMysqlDao : public CartonDataDao {
  public:
    CartonDataMysqlDao(const std::shared_ptr<zel::myorm::Database> &db, const std::string &order_name);
    ~CartonDataMysqlDao();

    /// @brief Batch insert new box datas into the database.
    bool batchAdd(const std::vector<std::shared_ptr<CartonData>> &carton_datas) override;

    /// @brief Get all carton datas from the database.
    std::vector<std::shared_ptr<CartonData>> all(const int &status = -1) override;

    /// @brief Set the status of the carton data to 1 when it is scanned.
    bool scanned(const std::string &start_barcode) override;

    /// @brief Get carton data from the database by start barcode.
    std::shared_ptr<CartonData> get(const std::string &start_barcode) override;

    /// @brief Update carton data in the database.
    bool update(const int &id, std::shared_ptr<CartonData> &carton_data) override;

    /// @brief Clear all data in the database.
    bool clear() override;

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<zel::myorm::Database> db_;
    std::string                           order_name_;
};