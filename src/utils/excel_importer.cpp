#include "excel_importer.h"
#include "data/box_data.h"
#include "data/carton_data.h"

#include <memory>
#include <vector>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/cell_vector.hpp>

namespace utils {

ExcelImporter::ExcelImporter(const std::string &box_file_path, const std::string &carton_file_path)
    : box_file_path_(box_file_path)
    , carton_file_path_(carton_file_path) {}

ExcelImporter::~ExcelImporter() {}

std::vector<std::string> ExcelImporter::boxHeaders() {
    std::vector<std::string> headers;

    xlnt::workbook wb;
    wb.load(box_file_path_);
    auto ws = wb.active_sheet();

    // get the first line content.
    auto first_row = ws.rows(false)[0];

    for (auto cell : first_row) {
        if (cell.has_value()) {
            headers.push_back(cell.to_string());
        } else {
            headers.push_back("");
        }
    }

    return headers;
}

std::vector<std::string> ExcelImporter::cartonHeaders() {
    std::vector<std::string> headers;

    xlnt::workbook wb;
    wb.load(carton_file_path_);
    auto ws = wb.active_sheet();

    // get the first line content.
    auto first_row = ws.rows(false)[0];

    for (auto cell : first_row) {
        if (cell.has_value()) {
            headers.push_back(cell.to_string());
        } else {
            headers.push_back("");
        }
    }

    return headers;
}

std::vector<std::shared_ptr<BoxData>> ExcelImporter::boxDatas(const std::shared_ptr<Format> &format) {
    std::vector<std::shared_ptr<BoxData>> box_datas;
    header_index_.clear();

    // Open Excel file
    xlnt::workbook workbook;
    workbook.load(box_file_path_);
    xlnt::worksheet worksheet = workbook.active_sheet();

    // Read table header（第一行）
    auto all_rows  = worksheet.rows(false);
    auto row_it    = all_rows.begin();
    int  col_index = 0;
    for (auto cell : *row_it) {
        // 尽量避免 to_string()，尝试直接获取字符串
        std::string header;
        if (cell.has_value()) {
            try {
                header = cell.value<std::string>();
            } catch (...) {
                header = cell.to_string(); // 兜底
            }
        }
        header_index_[header] = col_index++;
    }
    ++row_it;

    // 预分配容量（行数 - 1，除去表头）
    size_t row_count = worksheet.highest_row();
    if (row_count > 1) {
        box_datas.reserve(row_count - 1);
    }

    // 逐行读取数据
    int id_counter = 1;
    for (; row_it != all_rows.end(); ++row_it) {
        auto row = *row_it;

        auto box_data = std::make_shared<BoxData>();
        box_data->id  = id_counter++;

        box_data->filename     = getStr(row, format->filename);
        box_data->box_number   = getStr(row, format->box_number);
        box_data->start_number = getStr(row, format->start_number);
        box_data->end_number   = getStr(row, format->end_number);
        box_data->quantity     = getInt(row, format->quantity);

        std::vector<std::string> barcodes = split(getStr(row, format->barcode), ",");
        box_data->start_barcode           = barcodes[0];
        box_data->end_barcode             = barcodes.size() == 1 ? "" : barcodes[1];

        box_datas.push_back(std::move(box_data));
    }

    return box_datas;
}

std::vector<std::shared_ptr<CartonData>> ExcelImporter::cartonDatas(const std::shared_ptr<Format> &format) {
    std::vector<std::shared_ptr<CartonData>> carton_datas;
    header_index_.clear();

    // Open Excel file
    xlnt::workbook workbook;
    workbook.load(carton_file_path_);
    xlnt::worksheet worksheet = workbook.active_sheet();

    // Read table header（第一行）
    auto all_rows  = worksheet.rows(false);
    auto row_it    = all_rows.begin();
    int  col_index = 0;
    for (auto cell : *row_it) {
        // 尽量避免 to_string()，尝试直接获取字符串
        std::string header;
        if (cell.has_value()) {
            try {
                header = cell.value<std::string>();
            } catch (...) {
                header = cell.to_string(); // 兜底
            }
        }
        header_index_[header] = col_index++;
    }
    ++row_it;

    // 预分配容量（行数 - 1，除去表头）
    size_t row_count = worksheet.highest_row();
    if (row_count > 1) {
        carton_datas.reserve(row_count - 1);
    }

    // 逐行读取数据
    int id_counter = 1;
    for (; row_it != all_rows.end(); ++row_it) {
        auto row = *row_it;

        auto carton_data = std::make_shared<CartonData>();
        carton_data->id  = id_counter++;

        carton_data->filename      = getStr(row, format->filename);
        carton_data->carton_number = getStr(row, format->box_number);
        carton_data->start_number  = getStr(row, format->start_number);
        carton_data->end_number    = getStr(row, format->end_number);
        carton_data->quantity      = getInt(row, format->quantity);

        std::vector<std::string> barcodes = split(getStr(row, format->barcode), ",");
        carton_data->start_barcode        = barcodes[0];
        carton_data->end_barcode          = barcodes.size() == 1 ? "" : barcodes[1];

        carton_datas.push_back(std::move(carton_data));
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

std::vector<std::string> ExcelImporter::split(const std::string &str, const std::string &delim) {
    std::vector<std::string> ret;
    std::string::size_type   pos1, pos2;
    pos2 = str.find(delim);
    pos1 = 0;
    while (std::string::npos != pos2) {
        ret.push_back(str.substr(pos1, pos2 - pos1));

        pos1 = pos2 + delim.size();
        pos2 = str.find(delim, pos1);
    }
    if (pos1 != str.length()) ret.push_back(str.substr(pos1));

    return ret;
}

} // namespace utils