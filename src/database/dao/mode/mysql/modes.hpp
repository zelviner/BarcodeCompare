#pragma once

#include "database/myorm/database.h"
#include "database/myorm/model.hpp"

class Modes : public zel::myorm::Model<Modes> {
  public:
    Modes()
        : Model() {}
    Modes(zel::myorm::Database &db)
        : Model(db()) {}
    Modes(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return "modes"; }

    std::string primary_key() const { return "id"; }
};