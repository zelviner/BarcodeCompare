#pragma once

#include "database/dao/box_data/box_data.h"
#include "database/dao/carton_data/carton_data.h"
#include "database/dao/format/format.h"

#include <memory>
#include <string>
#include <vector>

// 导入器抽象接口类
class Importer {

  public:
    Importer(const std::string &box_file_path, const std::string &carton_file_path)
        : box_file_path_(box_file_path)
        , carton_file_path_(carton_file_path) {};

    ~Importer() = default;

    /// @brief Get the header data of box.
    virtual std::vector<std::string> boxHeaders() = 0;

    /// @brief Get the header data of carton.
    virtual std::vector<std::string> cartonHeaders() = 0;

    /// @brief Get the box data from tag data file.
    virtual std::vector<std::shared_ptr<BoxData>> boxDatas(const std::shared_ptr<Format> &format) = 0;

    /// @brief Get the carton data from tag data file.
    virtual std::vector<std::shared_ptr<CartonData>> cartonDatas(const std::shared_ptr<Format> &format) = 0;

  protected:
    static std::string remove_spaces(const std::string &s) {
        std::string result;
        result.reserve(s.size());
        for (char c : s) {
            if (c != ' ') result.push_back(c);
        }
        return result;
    }

    static std::vector<std::string> split(const std::string &str, const std::string &delim) {
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

  protected:
    std::string box_file_path_;    // tag file path of box data
    std::string carton_file_path_; // tag file path of carton data
};