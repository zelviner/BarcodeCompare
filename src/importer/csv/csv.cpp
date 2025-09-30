#include "csv.h"

#include <memory>
#include <qfile>
#include <qtextstream>
#include <string>
#include <vector>

std::vector<std::string> Csv::boxHeaders() {
    std::vector<std::string> headers;

    QFile box_file(QString::fromStdString(box_file_path_));
    if (!box_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return headers;
    }

    QTextStream in(&box_file);
    auto        line = in.readLine().toStdString();

    headers = split(line, ",");

    return headers;
}

std::vector<std::string> Csv::cartonHeaders() {
    std::vector<std::string> headers;

    QFile carton_file(QString::fromStdString(carton_file_path_));
    if (!carton_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return headers;
    }

    QTextStream in(&carton_file);
    auto        line = in.readLine().toStdString();

    headers = split(line, ",");

    return headers;
}

std::vector<std::shared_ptr<BoxData>> Csv::boxDatas(const std::shared_ptr<Format> &format) {
    std::vector<std::shared_ptr<BoxData>> box_datas;

    QFile box_file(QString::fromStdString(box_file_path_));
    if (!box_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return box_datas;
    }

    QTextStream in(&box_file);
    auto        line      = in.readLine().toStdString();
    auto        headers   = split(line, ",");
    int         col_index = 0;
    for (auto &header : headers) {
        header_index_[header] = col_index++;
    }

    int id_counter = 1;
    while (!in.atEnd()) {
        auto line  = in.readLine().toStdString();
        auto datas = split(line, ",");

        auto box_data = std::make_shared<BoxData>();
        box_data->id  = id_counter++;

        box_data->filename     = get_str(datas, format->filename);
        box_data->box_number   = get_str(datas, format->box_number);
        box_data->start_number = get_str(datas, format->start_number);
        box_data->end_number   = get_str(datas, format->end_number);
        box_data->quantity     = get_int(datas, format->quantity);

        if (format->barcode != "") {
            std::vector<std::string> barcodes = split(get_str(datas, format->barcode), ",");
            box_data->start_barcode           = barcodes[0];
            box_data->end_barcode             = barcodes.size() == 1 ? "" : barcodes[1];
        } else {
            box_data->start_barcode = get_str(datas, format->start_number);
            box_data->end_barcode   = get_str(datas, format->end_number);
        }

        box_datas.push_back(std::move(box_data));
    }

    return box_datas;
}

std::vector<std::shared_ptr<CartonData>> Csv::cartonDatas(const std::shared_ptr<Format> &format) {
    std::vector<std::shared_ptr<CartonData>> carton_datas;

    QFile carton_file(QString::fromStdString(carton_file_path_));
    if (!carton_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return carton_datas;
    }

    QTextStream in(&carton_file);
    auto        line      = in.readLine().toStdString();
    auto        headers   = split(line, ",");
    int         col_index = 0;
    for (auto &header : headers) {
        header_index_[header] = col_index++;
    }

    int id_counter = 1;
    while (!in.atEnd()) {
        auto line  = in.readLine().toStdString();
        auto datas = split(line, ",");

        auto carton_data = std::make_shared<CartonData>();
        carton_data->id  = id_counter++;

        carton_data->filename      = get_str(datas, format->filename);
        carton_data->carton_number = get_str(datas, format->box_number);
        carton_data->start_number  = get_str(datas, format->start_number);
        carton_data->end_number    = get_str(datas, format->end_number);
        carton_data->quantity      = get_int(datas, format->quantity);

        if (format->barcode != "") {
            std::vector<std::string> barcodes = split(get_str(datas, format->barcode), ",");
            carton_data->start_barcode        = barcodes[0];
            carton_data->end_barcode          = barcodes.size() == 1 ? "" : barcodes[1];
        } else {
            carton_data->start_barcode = get_str(datas, format->start_number);
            carton_data->end_barcode   = get_str(datas, format->end_number);
        }

        carton_datas.push_back(std::move(carton_data));
    }

    return carton_datas;
}

std::string Csv::get_str(const std::vector<std::string> &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < int(row.size())) {
        return removeSpaces(row[header_index_[header]]);
    }

    return "";
}

int Csv::get_int(const std::vector<std::string> &row, const std::string &header) {
    if (header_index_.count(header) && header_index_[header] < int(row.size())) {
        try {
            return std::stoi(row[header_index_[header]]);
        } catch (...) {
            return 0;
        }
    }

    return 0;
}