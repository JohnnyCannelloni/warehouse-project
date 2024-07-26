#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

using namespace std;

class Customer {
    public:
        Customer(int id, const string &name, int locationDistance, int maxOrders);
        Customer(const Customer& other);
        const string &getName() const;
        int getId() const;
        int getCustomerDistance() const;
        int getMaxOrders() const; 
        int getNumOrders() const; //Returns num of orders the customer did
        bool canMakeOrder() const; 
        const vector<int> &getOrdersIds() const;
        int addOrder(int orderId); 

        virtual Customer *clone() const = 0; 
        virtual ~Customer() = default;
        
        
    private:
        const int _id;
        const string _name;
        const int _locationDistance;
        const int _maxOrders;
        vector<int> _ordersId;
};


class SoldierCustomer: public Customer {
    public:
        SoldierCustomer(int id, const string &name, int locationDistance, int maxOrders);
        SoldierCustomer *clone() const override;
    
    private:
        
};

class CivilianCustomer: public Customer {
    public:
        CivilianCustomer(int id, const string &name, int locationDistance, int maxOrders);
        CivilianCustomer *clone() const override;
    
    private:
        
};