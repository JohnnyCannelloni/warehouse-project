#include "Action.h"
#include "Customer.h"
#include "Volunteer.h"
extern WareHouse* backup;

BaseAction::BaseAction() : _errorMsg(""), _status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const {
    return _status;
}

string BaseAction::ActionStatusToString(ActionStatus s) const {
    if (s == ActionStatus::COMPLETED)
        return "COMPLETED";
    else
        return "ERROR";
}

string BaseAction::OrderStatusToString(OrderStatus s) const {
    if (s == OrderStatus::PENDING)
        return "PENDING";
    else if(s == OrderStatus::COLLECTING)
        return "COLLECTING";
    else if (s == OrderStatus::DELIVERING)
        return "DELIVERING";
    else
        return "COMPLETED";
}

void BaseAction::complete() {
    _status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    _errorMsg = "Error: " + errorMsg;
    _status = ActionStatus::ERROR;
}

string BaseAction::getErrorMsg() const {
    return _errorMsg;
}

SimulateStep::SimulateStep(int numOfSteps) : _numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    for (int i = 0; i < _numOfSteps; i++) 
        performSingleStep(wareHouse);
}

void SimulateStep::performSingleStep(WareHouse& wareHouse) {
    vector<Order*>& pendingOrders = wareHouse.getPendingOrders();
    vector<Order*>& inProccessOrders = wareHouse.getInProccessOrders();
    vector<Order*>& completedOrders = wareHouse.getCompletedOrders();
    vector<Volunteer*>& volunteers = wareHouse.getVolunteers();

    //step 1
    for (int i = 0; i < pendingOrders.size(); i++) {
        Order* o = pendingOrders[i];
        //look for a collector
        if (o->getStatus() == OrderStatus::PENDING) {
            for (Volunteer* v : volunteers) {
                if (v->getType() == "Collector" && v->canTakeOrder(*o)) {
                    v->acceptOrder(*o);
                    o->setCollectorId(v->getId());
                    o->setStatus(OrderStatus::COLLECTING);
                    inProccessOrders.push_back(o);
                    pendingOrders[i] = nullptr;

                    break;
                }
            }
        }

        //look for a driver
        else if (o->getStatus() == OrderStatus::COLLECTING) {
            for (Volunteer* v : volunteers) {
                if (v->getType() == "Driver" && v->canTakeOrder(*o)) {
                    v->acceptOrder(*o);
                    o->setDriverId(v->getId());
                    o->setStatus(OrderStatus::DELIVERING);
                    inProccessOrders.push_back(o);
                    pendingOrders[i] = nullptr;

                    break;
                }
            }
        }
    }

    //Remove nullptr from pending orders
    for (int i = 0; i < pendingOrders.size(); i++) {
        if (pendingOrders[i] == nullptr) {
            pendingOrders.erase(pendingOrders.begin() + i);
            i--;
        }
    }

    //Rest of the steps
    for (int i = 0; i < volunteers.size(); i++) {
        Volunteer* v = volunteers[i];
        int pastCompletedOrder = v->getCompletedOrderId();
        
        if (v->getActiveOrderId() != NO_ORDER) {
            v->step();
            int newCompletedOrder = v->getCompletedOrderId();

            //a new order has been finished processing
            if (pastCompletedOrder != newCompletedOrder) {
                Order& order = wareHouse.getOrder(newCompletedOrder);
                
                //insert order back to pending
                if (v->getType() == "Collector") {
                    order.setStatus(OrderStatus::COLLECTING);
                    pendingOrders.push_back(&order);
                }

                //insert order to completed
                else if (v->getType() == "Driver") {
                    order.setStatus(OrderStatus::COMPLETED);
                    completedOrders.push_back(&order);
                }

                //Removing the order from in proccess
                for (int k = 0; k < inProccessOrders.size(); k++) {
                    if (inProccessOrders[k]->getId() == newCompletedOrder) 
                        inProccessOrders.erase(inProccessOrders.begin() + k);
                }

                if (!v->hasOrdersLeft()) {
                    delete v;
                    volunteers.erase(volunteers.begin() + i);
                }       
            }
        } 
    }
}

string SimulateStep::toString() const {
    return "step " + to_string(_numOfSteps) + " " + ActionStatusToString(getStatus());
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : _customerId(id) {

}

void AddOrder::act(WareHouse &wareHouse) {
    Customer& c = wareHouse.getCustomer(_customerId);
    if (c.getId() == -1 || !c.canMakeOrder()) {
        error("Cannot place this order");
        cout << getErrorMsg() << endl;
    } 
        
    else {
        Order* o = new Order(wareHouse.getOrderCounter(), _customerId, c.getCustomerDistance());
        wareHouse.addOrder(o);
        c.addOrder(o->getId());
        complete();
    }
}

string AddOrder::toString() const {
    return "order " + to_string(_customerId) + " " + ActionStatusToString(getStatus());
}
   
AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}
CustomerType AddCustomer::stringToType(string type) {
    if (type == "soldier")
        return CustomerType::Soldier;
    
    return CustomerType::Civilian;
}

string AddCustomer::typeToString(CustomerType type) const {
    if (type == CustomerType::Soldier)
        return "soldier";
    
    return "civilian";
}

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) : _customerName(customerName), _customerType(stringToType(customerType)), _distance(distance), _maxOrders(maxOrders) {

}

void AddCustomer::act(WareHouse &wareHouse) {
    if (_customerType == CustomerType::Soldier) 
        wareHouse.addCustomer(new SoldierCustomer(wareHouse.getCustomerCounter(), _customerName, _distance, _maxOrders));
    else
        wareHouse.addCustomer(new CivilianCustomer(wareHouse.getCustomerCounter(), _customerName, _distance, _maxOrders));       
    
    complete();
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    return "customer " + _customerName + " " + typeToString(_customerType) + " " + to_string(_distance) + " " + to_string(_maxOrders) + " " + ActionStatusToString(getStatus());
}

PrintOrderStatus::PrintOrderStatus(int id) : _orderId(id) {
    
}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    if (_orderId < 0 || _orderId >= wareHouse.getOrderCounter()) {
        error("Order doesn't exist");
        cout << getErrorMsg() << endl;
    }
        
    else {
        cout << wareHouse.getOrder(_orderId).toString() << endl;
        complete();
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const { 
    return "orderStatus " + to_string(_orderId) + " " + ActionStatusToString(getStatus());
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : _customerId(customerId) {

}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    Customer& c = wareHouse.getCustomer(_customerId);

    if (c.getId() < 0 || c.getId() >= wareHouse.getCustomerCounter()) {
        error("Customer doesn't exist");
        cout << getErrorMsg() << endl;
    }
        
    else {
        cout << "CustomerID: " + to_string(_customerId) << endl;
        
        for (int orderId : c.getOrdersIds()) {
            Order& o = wareHouse.getOrder(orderId);
            cout << "OrderId: " << orderId << endl;
            cout << "OrderStatus: " << OrderStatusToString(o.getStatus()) << endl;
        }
    
        cout << "numOrdersLeft: " + to_string(c.getMaxOrders() - c.getNumOrders()) << endl; 
        complete();
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus::toString() const { 
    return "customerStatus " + to_string(_customerId) + " " + ActionStatusToString(getStatus());
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : _volunteerId(id) {

}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer& v = wareHouse.getVolunteer(_volunteerId);
    if (v.getId() < 0 || v.getId() >= wareHouse.getVolunteerCounter()) {
        error("Volunteer doesn't exist");
        cout << getErrorMsg() << endl;
    }

    else {
        cout << v.toString() << endl;
        complete();
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    return "volunteerStatus " + to_string(_volunteerId) + " " + ActionStatusToString(getStatus());
}

PrintActionsLog::PrintActionsLog() {

}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (BaseAction* a : wareHouse.getActions()) 
        cout << a->toString() << endl;
    
    complete();
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log " + ActionStatusToString(getStatus());
}

Close::Close() {

}

void Close::act(WareHouse &wareHouse) {
    for (int i = 0; i < wareHouse.getOrderCounter(); i++) {
        Order o(wareHouse.getOrder(i));
        cout << "OrderID: " + to_string(o.getId()) + 
        ", CustomerID: " + to_string(o.getCustomerId()) + 
        ", Status: " + OrderStatusToString(o.getStatus()) << endl; 
    }

    wareHouse.close();
    complete();
}

Close* Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close " + ActionStatusToString(getStatus());
}

BackupWareHouse::BackupWareHouse() {

}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) 
        backup = new WareHouse(wareHouse);
    else
        *backup = wareHouse;
    
    complete();
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const {
    return "backup " + ActionStatusToString(getStatus());
}

RestoreWareHouse::RestoreWareHouse() {

}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("Error: No backup available");
        cout << getErrorMsg() << endl;
    }
    
    else {
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "restore " + ActionStatusToString(getStatus());
}