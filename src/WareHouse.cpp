#include "WareHouse.h"
#include "Action.h"

WareHouse::WareHouse(const string &configFilePath) 
    : _isOpen(true),
      _actionsLog(),
      _volunteers(),
      _pendingOrders(),
      _inProcessOrders(),
      _completedOrders(),
      _customers(),
      _customerCounter(0),
      _volunteerCounter(0),
      _orderCounter(0) {

    Parse parse;
    parse.parseConfigFile(configFilePath);

    _customerCounter = parse.getCustomers().size();
    _volunteerCounter = parse.getVolunteers().size();

    _volunteers = parse.getVolunteers();
    _customers = parse.getCustomers();
}

WareHouse::WareHouse(const WareHouse &other) :
    _isOpen(other._isOpen),
    _actionsLog(),
    _volunteers(),
    _pendingOrders(),
    _inProcessOrders(),
    _completedOrders(),
    _customers(),
    _customerCounter(other._customerCounter),
    _volunteerCounter(other._volunteerCounter),
    _orderCounter(other.getOrderCounter()) {

    for (BaseAction* a : other._actionsLog){
        _actionsLog.push_back(a->clone());
    }

    for (Volunteer* volunteer : other._volunteers){
        _volunteers.push_back(volunteer->clone());
    }

    for (Order* order : other._pendingOrders){
        _pendingOrders.push_back(order->clone());
    }

    for (Order* order : other._inProcessOrders){
        _inProcessOrders.push_back(order->clone());
    }

    for (Order* order : other._completedOrders){
        _completedOrders.push_back(order->clone());
    }

    for (Customer* customer : other._customers){
        _customers.push_back(customer->clone());
    }
}

WareHouse::~WareHouse() {
    cleanVectors();
}

WareHouse& WareHouse::operator=(const WareHouse& other) {
    if(&other != this){
        _isOpen = other._isOpen;
        _customerCounter = other._customerCounter;
        _volunteerCounter = other._volunteerCounter;
        cleanVectors();     
        _orderCounter = other.getOrderCounter();

        for (BaseAction* action : other._actionsLog){
            _actionsLog.push_back(action->clone());
        }

        for (Volunteer* volunteer : other._volunteers){
            _volunteers.push_back(volunteer->clone());
        }

        for (Order* order : other._pendingOrders){
            _pendingOrders.push_back(order->clone());
        }

        for (Order* order : other._inProcessOrders){
            _inProcessOrders.push_back(order->clone());
        }

        for (Order* order : other._completedOrders){
            _completedOrders.push_back(order->clone());
        }

        for (Customer* customer : other._customers){
            _customers.push_back(customer->clone());
        }   
    }
    return *this;
}

WareHouse::WareHouse(WareHouse&& other) noexcept :
    _isOpen(other._isOpen),
    _actionsLog(move(other._actionsLog)),
    _volunteers(move(other._volunteers)),
    _pendingOrders(move(other._pendingOrders)),
    _inProcessOrders(move(other._inProcessOrders)),
    _completedOrders(move(other._completedOrders)),
    _customers(move(other._customers)),
    _customerCounter(other._customerCounter),
    _volunteerCounter(other._volunteerCounter),
    _orderCounter(other._orderCounter) { }

WareHouse& WareHouse::operator=(WareHouse&& other) noexcept{

    if (this != &other) {
        cleanVectors(); // Clean up existing resources

        _isOpen = other._isOpen;
        _customerCounter = other._customerCounter;
        _volunteerCounter = other._volunteerCounter;
        _actionsLog = move(other._actionsLog);
        _volunteers = move(other._volunteers);
        _pendingOrders = move(other._pendingOrders);
        _inProcessOrders = move(other._inProcessOrders);
        _completedOrders = move(other._completedOrders);
        _customers = move(other._customers);
        _orderCounter = other._orderCounter;
    }

    return *this;
}

void WareHouse::cleanVectors() {
    for(BaseAction* action : _actionsLog){
        delete action;
    }
    _actionsLog.clear();

    for (Volunteer* volunteer : _volunteers){
        delete volunteer;
    }

    _volunteers.clear();

    for (Order* order : _pendingOrders){
        delete order;
    }

    _pendingOrders.clear();

    for (Order* order : _inProcessOrders){
        delete order;
    }

    _inProcessOrders.clear();

    for (Order* order : _completedOrders){
        delete order;
    }

    _completedOrders.clear();

    for (Customer* customer : _customers){
        delete customer;
    }

    _customers.clear();
}

void WareHouse::start() {
    std::cout << "Warehouse is open!" << std::endl;
    while (_isOpen)
    {
        BaseAction* action = nullptr;
        string input;
        string userAction;

        // Read a line of input from the user
        getline(cin, input);

        // Use stringstream to extract the first word
        stringstream ss(input);
        ss >> userAction;
        
        if(userAction == "step"){
            int steps;
            ss >> steps;
            action = new SimulateStep(steps);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "customer"){
            string customerName;
            string customerType;
            int distance;
            int maxOrders;
    
            ss >> customerName >> customerType >> distance >> maxOrders;
            
            action = new AddCustomer(customerName, customerType, distance, maxOrders);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "order"){
            int customerId;
            ss >> customerId;
            action = new AddOrder(customerId);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "orderStatus"){
            int orderId;
            ss >> orderId;
            action = new PrintOrderStatus(orderId);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "customerStatus"){
            int customerId;
            ss >> customerId;
            action = new PrintCustomerStatus(customerId);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "volunteerStatus"){
            int volunteerId;
            ss >> volunteerId;
            action = new PrintVolunteerStatus(volunteerId);
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "log"){
            action = new PrintActionsLog();
            action->act(*this);
            _actionsLog.push_back(action);  
        }

        else if(userAction == "close"){
            action = new Close();
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "backup"){
            action = new BackupWareHouse();
            action->act(*this);
            _actionsLog.push_back(action);
        }

        else if(userAction == "restore"){
            action = new RestoreWareHouse();
            action->act(*this);
            _actionsLog.push_back(action);
        }
    }
}

void WareHouse::addOrder(Order* order) {
    _pendingOrders.push_back(order);
    _orderCounter++;
}

void WareHouse::addAction(BaseAction* action) {
    _actionsLog.push_back(action);
}

void WareHouse::addCustomer(Customer* customer) {
    _customers.push_back(customer);
    _customerCounter++;
}

Customer& WareHouse::getCustomer(int customerId) const {
    for (Customer* c : _customers) 
        if (c->getId() == customerId)
            return *c;

    Customer* notFound = new CivilianCustomer(-1, "notFound", 0, 0);
    return *notFound;
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    for (Volunteer* v : _volunteers) 
        if (v->getId() == volunteerId)
            return *v;

    Volunteer* notFound = new CollectorVolunteer(-1, "notFound", 0);
    return *notFound;
}

Order& WareHouse::getOrder(int orderId) const {
    for (Order* o : _pendingOrders) 
        if (o->getId() == orderId)
            return *o;

    for (Order* o : _inProcessOrders) 
        if (o->getId() == orderId)
            return *o;

    for (Order* o : _completedOrders) 
        if (o->getId() == orderId)
            return *o;

    Order* notFound = new Order(-1, 0, 0);
    return *notFound;
}

int WareHouse::getOrderCounter() const {
    return _orderCounter;
}

int WareHouse::getCustomerCounter() const {
    return _customerCounter;
}

int WareHouse::getVolunteerCounter() const {
    return _volunteerCounter;
}

vector<Order*>& WareHouse::getPendingOrders() {
    return _pendingOrders;
}

vector<Order*>& WareHouse::getInProccessOrders() {
    return _inProcessOrders;
}

vector<Order*>& WareHouse::getCompletedOrders() {
    return _completedOrders;
}

vector<Volunteer*>& WareHouse::getVolunteers() {
    return _volunteers;
}

void WareHouse::incOrderCounter() {
    _orderCounter++;
}

void WareHouse::incCustomerCounter() {
    _customerCounter++;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return _actionsLog;
}

void WareHouse::close() {
    _isOpen = false;
}

void WareHouse::open() {
    _isOpen = true;
}
    


