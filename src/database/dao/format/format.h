#pragma once

#include <string>

struct Format {
    int         id;
    std::string name;
    int         type;         // box or carton, 1: box, 2: carton
    std::string filename;     // prd data filename
    std::string box_number;   // box number of the box
    std::string start_number; // start number of the box
    std::string end_number;   // end number of the box
    std::string quantity;     // quantity of the box
    std::string barcode;      // start barcode of the box
};
