#include "excel_importer.h"
#include "data/box_data.h"

#include <memory>
#include <vector>

namespace utils {

ExcelImporter::ExcelImporter(const std::string &box_file_path, const std::string &carton_file_path)
    : box_file_path_(box_file_path)
    , carton_file_path_(carton_file_path) {}

ExcelImporter::~ExcelImporter() {}

std::vector<std::shared_ptr<BoxData>> ExcelImporter::boxDatas() {
    std::vector<std::shared_ptr<BoxData>> box_datas;
    header_index_.clear();

    // Open Excel file
    xlnt::workbook workbook;
    workbook.load(box_file_path_);
    xlnt::worksheet worksheet = workbook.active_sheet();

    // Read table header
    int col_index = 0;
    for (auto cell : worksheet.rows(false).front()) {
        std::string header    = cell.to_string();
        header_index_[header] = col_index++;
    }

    int id_counter = 1;
    for (auto it = ++worksheet.rows(false).begin(); it != worksheet.rows(false).end(); ++it) {
        auto row      = *it;
        auto box_data = std::make_shared<BoxData>();
        box_data->id  = id_counter++;

        box_data->filename      = getStr(row, "No.WO/OF \n(数据文件名)");
        box_data->box_number    = getStr(row, "Inner Box No.\n（内盒号）");
        box_data->start_number  = getStr(row, "First SN\n（起始序列号）");
        box_data->end_number    = getStr(row, "Last SN\n（终止序列号）");
        box_data->quantity      = getInt(row, "Quantity\n（数量）");
        box_data->start_barcode = getStr(row, "Barcode\n（条形码）");
        box_data->end_barcode   = "";

        box_datas.push_back(box_data);
    }

    return box_datas;
}

std::vector<std::shared_ptr<CartonData>> ExcelImporter::cartonDatas() {
    std::vector<std::shared_ptr<CartonData>> carton_datas;
    header_index_.clear();

    // Open Excel file
    xlnt::workbook workbook;
    workbook.load(carton_file_path_);
    xlnt::worksheet worksheet = workbook.active_sheet();

    // Read table header
    int col_index = 0;
    for (auto cell : worksheet.rows(false).front()) {
        std::string header    = cell.to_string();
        header_index_[header] = col_index++;
    }

    int id_counter = 1;
    for (auto it = ++worksheet.rows(false).begin(); it != worksheet.rows(false).end(); ++it) {
        auto row         = *it;
        auto carton_data = std::make_shared<CartonData>();
        carton_data->id  = id_counter++;

        carton_data->filename      = getStr(row, "No.WO/OF \n(数据文件名)");
        carton_data->start_number  = getStr(row, "First SN\n（起始序列号）");
        carton_data->end_number    = getStr(row, "Last SN\n（终止序列号）");
        carton_data->quantity      = getInt(row, "Quantity\n（数量）");
        carton_data->start_barcode = getStr(row, "Barcode\n（条形码）");
        carton_data->end_barcode   = "";

        carton_datas.push_back(carton_data);
    }

    return carton_datas;
}

std::string ExcelImporter::getStr(const xlnt::cell_vector &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < row.length()) {
        return removeSpaces(row[header_index_[header]].to_string());
    }

    return "";
}

int ExcelImporter::getInt(const xlnt::cell_vector &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < row.length()) {
        try {
            return row[header_index_[header]].value<int>();
        } catch (...) {
            return 0;
        }
    }

    return 0;
}

std::string ExcelImporter::removeSpaces(const std::string &s) {
    std::string result;
    result.reserve(s.size());
    for (char c : s) {
        if (c != ' ') result.push_back(c);
    }
    return result;
}

} // namespace utils