#pragma once

#include "box_data.h"

#include <SQLiteCpp/Database.h>
#include <memory>
#include <vector>

class BoxDataDao {

  public:
    BoxDataDao(const std::shared_ptr<SQLite::Database> &sqlite_db, const std::string &order_name)
        : sqlite_db_(sqlite_db)
        , order_name_(order_name) {}
    ~BoxDataDao() {}

    /// @brief Batch insert new box datas into the database.
    virtual bool batchAdd(const std::vector<std::shared_ptr<BoxData>> &box_datas) = 0;

    /// @brief Get all box datas from the database.
    virtual std::vector<std::shared_ptr<BoxData>> all(const int &status = -1)                                         = 0;
    virtual std::vector<std::shared_ptr<BoxData>> all(const std::string &start_number, const std::string &end_number) = 0;

    /// @brief Set the status of the box data to 1 when it is scanned.
    virtual bool scanned(const std::string &start_barcode) = 0;

    /// @brief Get box data from the database by start barcode.
    virtual std::shared_ptr<BoxData> get(const std::string &start_barcode) = 0;

    /// @brief Update box data in the database.
    virtual bool update(const int &id, std::shared_ptr<BoxData> &box_data) = 0;

    /// @brief Clear all data in the database.
    virtual bool clear() = 0;

  protected:
    std::shared_ptr<SQLite::Database> sqlite_db_;
    std::string                       order_name_;
};