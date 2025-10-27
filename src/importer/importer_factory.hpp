#pragma once

#include "csv/csv.h"
#include "excel/excel.h"
#include "importer.hpp"

#include <memory>

// 导入器工厂类
class ImporterFactory {
  public:
    enum FileType { XLSX, CSV };

  public:
    ImporterFactory() {}
    ~ImporterFactory() {}

    static std::shared_ptr<Importer> create(FileType file_type, const std::string &box_file_path, const std::string &carton_file_path,
                                            const std::string &card_file_path) {
        switch (file_type) {
        case XLSX:
            return std::make_shared<Excel>(box_file_path, carton_file_path, card_file_path);

        case CSV:
            return std::make_shared<Csv>(box_file_path, carton_file_path, card_file_path);

        default:
            return std::make_shared<Excel>(box_file_path, carton_file_path, card_file_path);
        }
    }
};