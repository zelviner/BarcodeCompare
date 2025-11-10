#pragma once

#include <zel/myorm.h>

class CardTables : public zel::myorm::Model<CardTables> {
  public:
    CardTables()
        : Model() {}
    CardTables(zel::myorm::Database &db, const std::string &table_name, const std::string &primary_key)
        : Model(db())
        , table_name_(table_name)
        , primary_key_(primary_key) {}
    CardTables(zel::myorm::Connection *conn)
        : Model(conn) {}

    std::string table() const { return table_name_; }

    std::string primary_key() const { return primary_key_; }

  private:
    std::string table_name_;
    std::string primary_key_;
};