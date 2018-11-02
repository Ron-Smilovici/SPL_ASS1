#include "Table.h"

Table::Table(int t_capacity) : capacity(t_capacity) , open(1) {}

int Table::getCapacity() const
{
	return capacity;
}

void Table::addCustomer(Customer* customer) {}
void Table::removeCustomer(int id) {}
//Customer* getCustomer(int id);
//std::vector<Customer*>& getCustomers();
//std::vector<OrderPair>& getOrders();
//void order(const std::vector<Dish> &menu);

void Table::openTable() 
{
	open = true;
}

void Table::closeTable() 
{
	open = false;
}

//int getBill();
bool Table::isOpen() 
{
	return open;
}

std::string Table::toString() const
{
	return "capacity = " + capacity;
}