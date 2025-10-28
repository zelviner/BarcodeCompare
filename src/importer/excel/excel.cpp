#include "excel.h"

#include <memory>
#include <vector>
#include <xlnt/workbook/workbook.hpp>
#include <xlnt/worksheet/cell_vector.hpp>

std::vector<std::string> Excel::boxHeaders() {
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

std::vector<std::string> Excel::cartonHeaders() {
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

std::vector<std::string> Excel::cardHeaders() {
    std::vector<std::string> headers;

    if (card_file_path_ == "") {
        return headers;
    }

    xlnt::workbook wb;
    wb.load(card_file_path_);
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

std::vector<std::shared_ptr<BoxData>> Excel::boxDatas(const std::shared_ptr<Format> &format) {

    printf(" format->box_number: %s\n format->start_number: %s\n format->end_number: %s\n format->quantity: %s\n format->barcode: %s\n",
           format->box_number.c_str(), format->start_number.c_str(), format->end_number.c_str(), format->quantity.c_str(), format->barcode.c_str());

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

        box_data->filename     = get_str(row, format->filename);
        box_data->box_number   = get_str(row, format->box_number);
        box_data->start_number = get_str(row, format->start_number);
        box_data->end_number   = get_str(row, format->end_number);
        box_data->quantity     = get_int(row, format->quantity);

        if (format->barcode != "") {
            std::vector<std::string> barcodes = split(get_str(row, format->barcode), ",");
            box_data->start_barcode           = barcodes[0];
            box_data->end_barcode             = barcodes.size() == 1 ? "" : barcodes[1];
        } else {
            box_data->start_barcode = get_str(row, format->start_number);
            box_data->end_barcode   = get_str(row, format->end_number);
        }

        box_datas.push_back(std::move(box_data));
    }

    return box_datas;
}

std::vector<std::shared_ptr<CartonData>> Excel::cartonDatas(const std::shared_ptr<Format> &format) {
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

        carton_data->filename      = get_str(row, format->filename);
        carton_data->carton_number = get_str(row, format->box_number);
        carton_data->start_number  = get_str(row, format->start_number);
        carton_data->end_number    = get_str(row, format->end_number);
        carton_data->quantity      = get_int(row, format->quantity);

        if (format->barcode != "") {
            std::vector<std::string> barcodes = split(get_str(row, format->barcode), ",");
            carton_data->start_barcode        = barcodes[0];
            carton_data->end_barcode          = barcodes.size() == 1 ? "" : barcodes[1];
        } else {
            carton_data->start_barcode = get_str(row, format->start_number);
            carton_data->end_barcode   = get_str(row, format->end_number);
        }

        carton_datas.push_back(std::move(carton_data));
    }

    return carton_datas;
}

std::vector<std::shared_ptr<CardData>> Excel::cardDatas(const std::shared_ptr<Format> &format) {
    std::vector<std::shared_ptr<CardData>> card_datas;
    header_index_.clear();

    // Open Excel file
    xlnt::workbook workbook;
    workbook.load(card_file_path_);
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
        card_datas.reserve(row_count - 1);
    }

    // 逐行读取数据
    int id_counter = 1;
    for (; row_it != all_rows.end(); ++row_it) {
        auto row = *row_it;

        auto card_data = std::make_shared<CardData>();
        card_data->id  = id_counter++;

        card_data->card_number   = get_str(row, format->box_number);
        card_data->iccid         = get_str(row, format->start_number);
        card_data->imsi_barcode  = get_str(row, format->end_number);
        card_data->iccid_barcode = get_str(row, format->start_number);
        card_data->imsi_barcode  = get_str(row, format->end_number);
        card_data->quantity      = get_int(row, format->quantity);

        card_datas.push_back(std::move(card_data));
    }

    return card_datas;
}

std::string Excel::get_str(const xlnt::cell_vector &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < int(row.length())) {
        return remove_spaces(row[header_index_[header]].to_string());
    }

    return "";
}

int Excel::get_int(const xlnt::cell_vector &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < int(row.length())) {
        try {
            return row[header_index_[header]].value<int>();
        } catch (...) {
            return 0;
        }
    }

    return 0;
}