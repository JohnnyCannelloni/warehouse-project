#include "Volunteer.h"

Volunteer::Volunteer(int id, const string &name) : _completedOrderId(NO_ORDER), _activeOrderId(NO_ORDER), _id(id), _name(name) 
{
    
}

int Volunteer::getId() const {
    return _id;
}

const string& Volunteer::getName() const {
    return _name;
}

const string& Volunteer::getType() const {
    return _type;
}

int Volunteer::getActiveOrderId() const {
    return _activeOrderId;
}

int Volunteer::getCompletedOrderId() const {
    return _completedOrderId;
}

bool Volunteer::isBusy() const {
    return (_activeOrderId != NO_ORDER);
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), _coolDown(coolDown), _timeLeft(coolDown) { 
_type = "Collector";
}

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step() {
    if (decreaseCoolDown()) {
        _completedOrderId = _activeOrderId;
        _activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const {
    return _coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return _timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    _timeLeft--;

    return (_timeLeft == 0);
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy();
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    if (canTakeOrder(order)) {
        _activeOrderId = order.getId();
        _timeLeft = _coolDown;
    }
}

string CollectorVolunteer::toString() const {
    string res;
    string busy;

    res += "VolunteerID: " + to_string(getId()) + "\n";

    if (isBusy()) {
        busy += "true";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: " + to_string(getActiveOrderId()) + "\n";
        res += "Time Left: " + to_string(getTimeLeft()) + "\n";
    }
        
    else {
        busy += "false";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: None\n";
        res += "Time Left: None\n";
    }
    
    res += "Orders Left: No Limit";

    return res;
}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders) : CollectorVolunteer(id, name, coolDown), _maxOrders(maxOrders), _ordersLeft(maxOrders) {
_type = "Collector";
}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return (_ordersLeft > 0);
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return (CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft());
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) { 
    CollectorVolunteer::acceptOrder(order);
    _ordersLeft--; 
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return _maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return _ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    string res;
    string busy;

    res += "VolunteerID: " + to_string(getId()) + "\n";

    if (isBusy()) {
        busy += "true";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: " + to_string(getActiveOrderId()) + "\n";
        res += "Time Left: " + to_string(getTimeLeft()) + "\n";
    }
        
    else {
        busy += "false";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: None\n";
        res += "Time Left: None\n";
    }
    
    res += "Orders Left: " + to_string(_ordersLeft);

    return res;
}

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) : Volunteer(id, name), _maxDistance(maxDistance), _distancePerStep(distancePerStep), _distanceLeft(0) {
_type = "Driver";
}

DriverVolunteer* DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return _distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return _maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return _distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    _distanceLeft -= _distancePerStep;

    return (_distanceLeft <= 0);
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return (_maxDistance >= order.getDistance() && !isBusy());
}

void DriverVolunteer::acceptOrder(const Order &order) {
    if (canTakeOrder(order)) {
        _activeOrderId = order.getId();
        _distanceLeft = order.getDistance();
    }
}

void DriverVolunteer::step() {
    if (decreaseDistanceLeft()) {
        _completedOrderId = _activeOrderId;
        _activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const {
    string res;
    string busy;

    res += "VolunteerID: " + to_string(getId()) + "\n";

    if (isBusy()) {
        busy += "true";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: " + to_string(getActiveOrderId()) + "\n";
        res += "Distance Left: " + to_string(getDistanceLeft()) + "\n";
    }
        
    else {
        busy += "false";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: None\n";
        res += "Time Left: None\n";
    }
    
    res += "Orders Left: No Limit";

    return res;
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) : DriverVolunteer(id, name, maxDistance, distancePerStep), _maxOrders(maxOrders), _ordersLeft(maxOrders) {
_type = "Driver";
}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return _maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return _ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return (_ordersLeft > 0);
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return (DriverVolunteer::canTakeOrder(order) && hasOrdersLeft());
}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    if (canTakeOrder(order)) {
        DriverVolunteer::acceptOrder(order);
        _ordersLeft--;
    }
}

string LimitedDriverVolunteer::toString() const {
    string res;
    string busy;

    res += "VolunteerID: " + to_string(getId()) + "\n";

    if (isBusy()) {
        busy += "true";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: " + to_string(getActiveOrderId()) + "\n";
        res += "Distance Left: " + to_string(getDistanceLeft()) + "\n";
    }
        
    else {
        busy += "false";
        res += "isBusy: " + busy + "\n";
        res += "OrderId: None\n";
        res += "Time Left: None\n";
    }
    
    res += "Orders Left: " + to_string(_ordersLeft);

    return res;
}

