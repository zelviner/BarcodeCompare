#include "order.h"

#include <qfile>
#include <qjsonarray>
#include <qjsondocument>
#include <qjsonobject>
#include <qjsonvalue>
#include <qobject.h>
#include <qvector>

const QVector<QString> Order::mode_ = {QObject::tr("起始条码和结束条码"), QObject::tr("仅有起始条码")};

Order::Order(QString filename)
    : filename_(filename) {
    load();
}

Order::~Order() {}

void Order::print() const {
    for (auto order : orders_) {
        printf("order_name: %s, check_format: %s, create_time: %s\n", order.order_name.toStdString().c_str(), order.check_format.toStdString().c_str(),
               order.create_time.toStdString().c_str());
    }
}

const Order::OrderVector &Order::orders() const { return orders_; }

const QStringList &Order::nameList() {
    name_list_.clear();
    for (auto order : orders_) {
        name_list_.push_back(order.order_name);
    }
    return name_list_;
}

bool Order::currentOrder(const QString &current_order) {
    for (auto order : orders_) {
        if (order.order_name == current_order) {
            current_order_ = order;
            return true;
        }
    }
    return false;
}

const Order::OrderInfo &Order::currentOrder() const { return current_order_; }

bool Order::add(const OrderInfo &order_info) {
    orders_.push_back(order_info);
    return save();
}

bool Order::remove(const QString &order_name) {
    for (auto it = orders_.begin(); it != orders_.end(); ++it) {
        if (it->order_name == order_name) {
            orders_.erase(it);
            break;
        }
    }

    return save();
}

bool Order::clearBoxScannedNum() {
    for (auto &order : orders_) {
        if (order.order_name == current_order_.order_name) {
            order.box_scanned_num = 0;
        }
    }

    return save();
}

bool Order::clearCartonScannedNum() {
    for (auto &order : orders_) {
        if (order.order_name == current_order_.order_name) {
            order.carton_scanned_num = 0;
        }
    }
    return save();
}

bool Order::clear() {
    orders_.clear();
    return save();
}

bool Order::update(const QString &order_name, const OrderInfo &order_info) {
    for (auto it = orders_.begin(); it != orders_.end(); ++it) {
        if (it->order_name == order_name) {
            int scanned_num     = it->box_scanned_num;
            *it                 = order_info;
            it->box_scanned_num = scanned_num;
            break;
        }
    }

    return save();
}
bool Order::updateCarton(const QString &order_name, const OrderInfo &order_info) {
    for (auto it = orders_.begin(); it != orders_.end(); ++it) {
        if (it->order_name == order_name) {
            int scanned_num        = it->carton_scanned_num;
            *it                    = order_info;
            it->carton_scanned_num = scanned_num;
            break;
        }
    }

    return save();
}

bool Order::scanned() {
    for (auto &order : orders_) {
        if (order.order_name == current_order_.order_name) {
            order.box_scanned_num++;
            current_order_.box_scanned_num++;
            break;
        }
    }
    return save();
}

bool Order::scannedCarton() {
    for (auto &order : orders_) {
        if (order.order_name == current_order_.order_name) {
            order.carton_scanned_num++;
            current_order_.carton_scanned_num++;
            break;
        }
    }
    return save();
}

bool Order::load() {
    QFile file(filename_);
    if (!file.open(QIODevice::ReadOnly)) {
        printf("open file %s failed\n", filename_.toStdString().c_str());
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument   json_doc(QJsonDocument::fromJson(data, &json_error));
    if (json_error.error != QJsonParseError::NoError) {
        printf("json error\n");
        return false;
    }

    QJsonObject obj = json_doc.object();
    QJsonArray  array;
    if (obj.contains("orders")) {
        array = obj["orders"].toArray();
    }

    for (int i = 0; i < array.size(); ++i) {
        QJsonObject order_obj = array[i].toObject();
        OrderInfo   order_info;
        order_info.order_name             = order_obj["order_name"].toString();
        order_info.carton_count           = order_obj["carton_count"].toInt();
        order_info.box_count              = order_obj["box_count"].toInt();
        order_info.check_format           = order_obj["check_format"].toString();
        order_info.create_time            = order_obj["create_time"].toString();
        order_info.carton_start_check_num = order_obj["carton_start_check_num"].toInt();
        order_info.carton_end_check_num   = order_obj["carton_end_check_num"].toInt();
        order_info.box_start_check_num    = order_obj["box_start_check_num"].toInt();
        order_info.box_end_check_num      = order_obj["box_end_check_num"].toInt();
        order_info.card_start_check_num   = order_obj["card_start_check_num"].toInt();
        order_info.card_end_check_num     = order_obj["card_end_check_num"].toInt();
        order_info.box_scanned_num        = order_obj["box_scanned_num"].toInt();
        order_info.carton_scanned_num     = order_obj["carton_scanned_num"].toInt();
        order_info.barcode_mode           = static_cast<Mode>(order_obj["barcode_mode"].toInt());
        orders_.push_back(order_info);
    }

    return true;
}

bool Order::save() {
    QJsonObject obj;
    QJsonArray  array;
    for (auto order : orders_) {
        QJsonObject order_obj;
        order_obj.insert("order_name", order.order_name);
        order_obj.insert("carton_count", order.carton_count);
        order_obj.insert("box_count", order.box_count);
        order_obj.insert("check_format", order.check_format);
        order_obj.insert("create_time", order.create_time);
        order_obj.insert("barcode_mode", static_cast<int>(order.barcode_mode));
        order_obj.insert("carton_start_check_num", order.carton_start_check_num);
        order_obj.insert("carton_end_check_num", order.carton_end_check_num);
        order_obj.insert("box_start_check_num", order.box_start_check_num);
        order_obj.insert("box_end_check_num", order.box_end_check_num);
        order_obj.insert("card_start_check_num", order.card_start_check_num);
        order_obj.insert("card_end_check_num", order.card_end_check_num);
        order_obj.insert("box_scanned_num", order.box_scanned_num);
        order_obj.insert("carton_scanned_num", order.carton_scanned_num);
        array.append(order_obj);
    }
    obj.insert("orders", array);

    QJsonDocument json_doc(obj);
    QByteArray    data = json_doc.toJson();

    QFile file(filename_);
    if (!file.open(QIODevice::WriteOnly)) {
        printf("open file %s failed\n", filename_.toStdString().c_str());
        return false;
    }

    file.write(data);
    file.close();

    return true;
}
