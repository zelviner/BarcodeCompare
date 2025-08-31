#pragma once

#include "carton_data.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class CartonDataDao {
  public:
    CartonDataDao(const std::shared_ptr<SQLite::Database> &db, const std::string &order_name);
    ~CartonDataDao();

    /// @brief Batch insert new box datas into the database.
    bool batchAdd(const std::vector<std::shared_ptr<CartonData>> &carton_datas);

    /// @brief Get all box datas from the database.
    std::vector<std::shared_ptr<CartonData>> all(const int &status = -1);

    /// @brief Set the status of the carton data to 1 when it is scanned.
    bool scanned(const std::string &start_barcode);

    /// @brief Get box data from the database by start barcode.
    std::shared_ptr<CartonData> get(const std::string &start_barcode);

    /// @brief Update box data in the database.
    bool update(const int &id, std::shared_ptr<CartonData> &carton_data);

    /// @brief Clear all data in the database.
    bool clear();

  private:
    /// @brief Initialize the data and create tables.
    void init();

  private:
    std::shared_ptr<SQLite::Database> db_;
    std::string                       order_name_;
};