#pragma once

#include <string>

struct CartonData {
    int         id;
    std::string filename;      // prd data filename
    std::string carton_number; // carton number of carton
    std::string start_number;  // start number of the carton
    std::string end_number;    // end number of the carton
    int         quantity;      // quantity of the carton
    std::string start_barcode; // start barcode of the carton
    std::string end_barcode;   // end barcode of the carton
    int         status;        // scan status of thecarton, 0: not scanned, 1: scanned, 2: scanned with error

    // std::vector<std::shared_ptr<BoxData>> boxes; // boxes in the carton
};