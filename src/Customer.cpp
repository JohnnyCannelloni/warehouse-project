#include "Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) : _id(id), _name(name), _locationDistance(locationDistance), _maxOrders(maxOrders){
    
}

Customer::Customer(const Customer& other) : _id(other._id), _name(other._name), _locationDistance(other._locationDistance), _maxOrders(other._maxOrders), _ordersId(other._ordersId) {

}

const string& Customer::getName() const {
    return _name;
}

int Customer::getId() const {
    return _id;
}

int Customer::getCustomerDistance() const {
    return _locationDistance;
}

 int Customer::getMaxOrders() const {
    return _maxOrders;
 }

 int Customer::getNumOrders() const {
    return _ordersId.size();
 }

 bool Customer::canMakeOrder() const {
    return (getNumOrders() < getMaxOrders());
 }

 const vector<int>& Customer::getOrdersIds() const {
    return _ordersId;
 }

 int Customer::addOrder(int orderId) {
    if (canMakeOrder()) {
        _ordersId.push_back(orderId);
        return orderId;
    }

    return -1;   
 }

SoldierCustomer::SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) { }

SoldierCustomer* SoldierCustomer::clone() const {
   return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders) : Customer(id, name, locationDistance, maxOrders) { }

CivilianCustomer* CivilianCustomer::clone() const {
   return new CivilianCustomer(*this);
}


