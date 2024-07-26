#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"
#include "Parse.h"

class BaseAction;
class Volunteer;

using namespace std;
// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse(const WareHouse &other);
        ~WareHouse();
        WareHouse& operator=(const WareHouse& other);
        WareHouse(WareHouse&& other) noexcept;
        WareHouse& operator=(WareHouse&& other) noexcept;
        void cleanVectors();
        void start();
        void addOrder(Order* order); 
        void addAction(BaseAction* action);
        void addCustomer(Customer* customer);
        Customer &getCustomer(int customerId) const; //return id -1 if no customer found bappp
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const; 
        int getOrderCounter() const;
        int getCustomerCounter() const;
        int getVolunteerCounter() const;
        vector<Order*>& getPendingOrders();
        vector<Order*>& getInProccessOrders();
        vector<Order*>& getCompletedOrders();
        vector<Volunteer*>& getVolunteers();
        void incOrderCounter();
        void incCustomerCounter();
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

    private:
        bool _isOpen;
        vector<BaseAction*> _actionsLog;
        vector<Volunteer*> _volunteers;
        vector<Order*> _pendingOrders;
        vector<Order*> _inProcessOrders;
        vector<Order*> _completedOrders;
        vector<Customer*> _customers;
        int _customerCounter; //For assigning unique customer IDs
        int _volunteerCounter; //For assigning unique volunteer IDs
        int _orderCounter; //For assigning unique volunteer IDs
};