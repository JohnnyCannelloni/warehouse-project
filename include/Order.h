#pragma once
#include <iostream>
#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace std;

enum class OrderStatus {
    PENDING,
    COLLECTING,
    DELIVERING,
    COMPLETED,
};

#define NO_VOLUNTEER -1

class Order {

    public:
        Order(int id, int customerId, int distance);
        int getId() const;
        int getCustomerId() const;
        void setStatus(OrderStatus status);
        void setCollectorId(int collectorId);
        void setDriverId(int driverId);
        int getCollectorId() const;
        int getDriverId() const;
        OrderStatus getStatus() const;
        int getDistance() const;
        Order* clone() const;
        const string toString() const;

    private:
        const int _id;
        const int _customerId;
        const int _distance;
        OrderStatus _status;
        int _collectorId; //Initialized to NO_VOLUNTEER if no collector has been assigned yet
        int _driverId; //Initialized to NO_VOLUNTEER if no driver has been assigned yet
};