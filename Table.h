#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table {
public:
	Table(const Table &table);//copy constructor
	Table(Table &&table);//move constructor
	Table& operator=(const Table & table); //copy assignment operator
	Table& operator=(Table && table);//move assignment operator
	Table(int t_capacity);
	int getCapacity() const;
	void addCustomer(Customer* customer);
	void removeCustomer(int id);
	Customer* getCustomer(int id);
	std::vector<Customer*>& getCustomers();
	std::vector<OrderPair>& getOrders();
	void order(const std::vector<Dish> &menu);
	void openTable();
	void closeTable();
	int getBill();
	bool isOpen();
	std::string toString() const;
	std::vector<OrderPair> getCustomerOrders(int customer_id);
	void updateOrders(std::vector<OrderPair> orders);

private:
	int capacity;
	bool open;
	std::vector<Customer*> customersList;
	std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif