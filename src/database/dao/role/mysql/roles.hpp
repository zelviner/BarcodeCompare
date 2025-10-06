#pragma once

#include "database/myorm/database.h"
#include "database/myorm/model.hpp"

class Roles : public zel::myorm::Model<Roles> {
  public:
    Roles()
        : Model() {}
    Roles(zel::myorm::Database &db)
        : Model(db()) {}
    Roles(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return "roles"; }

    std::string primary_key() const { return "id"; }
};