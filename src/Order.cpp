#include "Order.h"

Order::Order(int id, int customerId, int distance) : _id(id), _customerId(customerId), _distance(distance), _status(OrderStatus::PENDING), _collectorId(NO_VOLUNTEER), _driverId(NO_VOLUNTEER) {

}

int Order::getId() const {
    return _id;
}

int Order::getCustomerId() const {
    return _customerId;
}

void Order::setStatus(OrderStatus status) {
    _status = status;
}

void Order::setCollectorId(int collectorId) {
    _collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    _driverId = driverId;
}

int Order::getCollectorId() const {
    return _collectorId;
}

int Order::getDriverId() const {
    return _driverId;
}

OrderStatus Order::getStatus() const {
    return _status;
}

int Order::getDistance() const {
    return _distance;
}

Order* Order::clone() const {
    return new Order(*this);
}

const string Order::toString() const {
    string res = "OrderId: " + to_string(_id) + "\n";
    res += "OrderStatus: ";
    
    switch (_status)
    {
    case OrderStatus::PENDING:
        res += "PENDING\n";
        break;
    case OrderStatus::COLLECTING:
        res += "COLLECTING\n";
        break;
    case OrderStatus::DELIVERING:
        res += "DELIVERING\n";
        break;
    case OrderStatus::COMPLETED:
        res += "COMPLETED\n";
        break;
    
    default:
        break;
    }

    res += "CustomerId: " + to_string(_customerId) + "\n";
    
    if (_collectorId == NO_VOLUNTEER)
        res += "Collector: None\n";
    else
        res += "Collector: " + to_string(_collectorId) + "\n";

    if (_driverId == NO_VOLUNTEER)
        res += "Driver: None\n";
    else
        res += "Driver: " + to_string(_driverId) + "\n";

    return res;
}
