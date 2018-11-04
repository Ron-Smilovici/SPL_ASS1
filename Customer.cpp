#include "Customer.h"

// Customer
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) 
{
	
}

std::string Customer::getName() const { return name; }
int Customer::getId() const { return id; }

// CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string CheapCustomer::toString() const { return "test"; }

// VegetarianCustomer
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {}
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string VegetarianCustomer::toString() const { return "test"; }

// SpicyCustomer
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) {}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string SpicyCustomer::toString() const { return "test"; }

// AlchoholicCustomer
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name,id) {}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string AlchoholicCustomer::toString() const { return "test"; }