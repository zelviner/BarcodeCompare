#pragma once

#include "database/myorm/database.h"
#include "database/myorm/model.hpp"

class Formats : public zel::myorm::Model<Formats> {
  public:
    Formats()
        : Model() {}
    Formats(zel::myorm::Database &db)
        : Model(db()) {}
    Formats(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return "formats"; }

    std::string primary_key() const { return "id"; }
};