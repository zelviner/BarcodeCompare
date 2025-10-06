#pragma once

#include "database/myorm/database.h"
#include "database/myorm/model.hpp"

class Users : public zel::myorm::Model<Users> {
  public:
    Users()
        : Model() {}
    Users(zel::myorm::Database &db)
        : Model(db()) {}
    Users(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return "users"; }

    std::string primary_key() const { return "id"; }
};