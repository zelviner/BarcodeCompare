#pragma once

#include <zel/myorm.h>

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