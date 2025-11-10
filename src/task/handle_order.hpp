#pragma once

#include "database/order/order_dao.h"
#include <memory>
#include <qobject>
#include <qthread>

class HandleOrder : public QThread {
    Q_OBJECT

  public:
    HandleOrder(const std::shared_ptr<OrderDao> &order_dao, const std::shared_ptr<Order> &order)
        : order_dao_(order_dao)
        , order_(order) {}

    void run() override {
        // 添加订单
        if (order_dao_->add(order_)) {
            emit addOrderSuccess();
        } else {
            emit addOrderFailure();
        }
    }

  signals:
    // 信号函数，用于向外界发射信号
    void addOrderSuccess();
    void addOrderFailure();

  private:
    std::shared_ptr<OrderDao> order_dao_;
    std::shared_ptr<Order>    order_;
};