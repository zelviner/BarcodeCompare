#pragma once

#include "importer/importer.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <xlnt/xlnt.hpp>

class Excel : public Importer {
  public:
    // 继承父类构造函数
    using Importer::Importer;

    /// @brief Get the header data of box.
    std::vector<std::string> boxHeaders() override;

    /// @brief Get the header data of carton.
    std::vector<std::string> cartonHeaders() override;

    /// @brief Get the box data from tag data file.
    std::vector<std::shared_ptr<BoxData>> boxDatas(const std::shared_ptr<Format> &format) override;

    /// @brief Get the carton data from tag data file.
    std::vector<std::shared_ptr<CartonData>> cartonDatas(const std::shared_ptr<Format> &format) override;

  private:
    std::string get_str(const xlnt::cell_vector &row, const std::string &header);
    int         get_int(const xlnt::cell_vector &row, const std::string &header);

  private:
    std::unordered_map<std::string, int> header_index_;
};
