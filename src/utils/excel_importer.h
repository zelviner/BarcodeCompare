#pragma once

#include "data/box_data.h"
#include "data/carton_data.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <xlnt/xlnt.hpp>

namespace utils {

class ExcelImporter {
  public:
    ExcelImporter(const std::string &box_file_path, const std::string &carton_file_path);
    ~ExcelImporter();

    std::vector<std::shared_ptr<BoxData>> boxDatas();

    std::vector<std::shared_ptr<CartonData>> cartonDatas();

  private:
    std::string getStr(const xlnt::cell_vector &row, const std::string &header);

    int getInt(const xlnt::cell_vector &row, const std::string &header);

    std::string removeSpaces(const std::string &s);

  private:
    std::string                          box_file_path_;    // tag file path of box data
    std::string                          carton_file_path_; // tag file path of carton data
    std::unordered_map<std::string, int> header_index_;
};

} // namespace utils