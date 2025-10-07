#pragma once

#include "database/myorm/database.h"
#include "database/myorm/model.hpp"

class Orders : public zel::myorm::Model<Orders> {
  public:
    Orders()
        : Model() {}
    Orders(zel::myorm::Database &db)
        : Model(db()) {}
    Orders(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return "orders"; }

    std::string primary_key() const { return "id"; }
};