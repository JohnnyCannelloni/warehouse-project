#pragma once
#include <string>
#include <vector>
#include "WareHouse.h"

class Parse {
public:
    Parse();
    void parseConfigFile(const std::string& configFilePath);
    std::vector<Customer*>& getCustomers();
    std::vector<Volunteer*>& getVolunteers();

private:
    void parseCustomerLine(const std::string& line);
    void parseVolunteerLine(const std::string& line);

    std::vector<Customer*> customers;
    std::vector<Volunteer*> volunteers;
    int customerCounter;
    int volunteerCounter;
};
