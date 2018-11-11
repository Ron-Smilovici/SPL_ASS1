#include "Table.h"

Table::Table(int t_capacity) : capacity(t_capacity) , open(false) {}
//copy constuctor
Table::Table(const Table &table)
{
    open = table.open;
    capacity = table.capacity;

	for (int i = 0; i < table.orderList.size(); ++i) {
		orderList.push_back(table.orderList.at(i));
	}

	for(int i = 0; i < table.customersList.size(); ++i){
		Customer* temp = table.customersList.at(i);
		Customer* cloned = temp->clone();
		customersList.push_back(cloned);
	}
}
//copy assignment operator
Table& Table::operator=(const Table &table) {
	open = table.open;
	capacity = table.capacity;

	for (int i = 0; i < table.orderList.size(); ++i) {
		orderList.push_back(table.orderList.at(i));
	}

	for(int i = 0; i < table.customersList.size(); ++i){
		Customer* temp = table.customersList.at(i);
		Customer* cloned = temp->clone();
		customersList.push_back(cloned);
	}
}

//move constructor
Table::Table(Table &&table)
{
	open = table.open;
	capacity = table.capacity;

	for (int i = 0; i < table.orderList.size(); ++i) {
		orderList.push_back(table.orderList.at(i));
	}

	for(int i = 0; i < table.customersList.size(); ++i){
		customersList.push_back(table.customersList.at(i));
		table.customersList.at(i) = nullptr;
	}
}

//move assignment operator
Table& Table::operator=(Table &&table) {
	open = table.open;
	capacity = table.capacity;

	for (int i = 0; i < table.orderList.size(); ++i) {
		orderList.push_back(table.orderList.at(i));
	}

	for(int i = 0; i < table.customersList.size(); ++i){
		customersList.push_back(table.customersList.at(i));
		table.customersList.at(i) = nullptr;
	}
}

int Table::getCapacity() const
{
	return capacity;
}

void Table::addCustomer(Customer* customer) 
{
	if (!customer)
		return;
	this->customersList.push_back(customer);
}

void Table::removeCustomer(int id) 
{
	this->customersList.erase(customersList.begin() + id);
}

Customer* Table::getCustomer(int id)
{
	if ((id < 0) || (id >= this->getCapacity()))
		return NULL;
	return this->customersList.at(id);
}

std::vector<Customer*>& Table::getCustomers()
{
	return this->customersList;
}

std::vector<OrderPair>& Table::getOrders()
{
	return this->orderList;
}

void Table::order(const std::vector<Dish> &menu)
{
	std::vector<Customer*> vec_customers;

	vec_customers = this->getCustomers();
	for (std::vector<Customer*>::const_iterator i = vec_customers.begin(); i != vec_customers.end(); ++i)
	{
		// Execute order for each customer i in the table and return a vector
		// of dishes id that the csutomer ordered
		std::vector<int> ordered_dishes_id = (*i)->order(menu);

		// Create pairs for all the dishes customer i ordered
		for (std::vector<int>::const_iterator j = ordered_dishes_id.begin(); j != ordered_dishes_id.end(); ++j)
		{
			pair <int, Dish> ordered_dish(*j, menu.at(*j));
			// push the pair to vector orderList
			(this->getOrders()).push_back(ordered_dish);
			cout << (*i)->getName() << " ordered " << menu.at(*j).getName() << endl;
		}
	}
}

void Table::openTable() 
{
	open = true;
}

void Table::closeTable() 
{
	open = false;
}

int Table::getBill()
{
	int sum = 0;
	std::vector<OrderPair> op = this->getOrders();

	for (std::vector<OrderPair>::const_iterator i = op.begin(); i != op.end(); ++i)
	{
		cout << "Adding to bill dish " << (*i).second.getName() << " price " << (*i).second.getPrice() << endl;
		sum += (*i).second.getPrice();
	}

	return sum;
}

bool Table::isOpen() 
{
	return open;
}

std::string Table::toString() const
{
	return "capacity = " + capacity;
}