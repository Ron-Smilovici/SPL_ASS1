#include "Customer.h"

// Customer
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) 
{
	


}
//std::vector<int> Customer::order(const std::vector<Dish> &menu) {}
//std::string Customer::toString() const {}
std::string Customer::getName() const { return name; }
int Customer::getId() const { return id; }