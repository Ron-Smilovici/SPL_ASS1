#include "Action.h"
#include "Restaurant.h" // Added becuase the use of all restaurant methods... check with someone else.
BaseAction::BaseAction() 
{

}

// make sure regarding the copy constuctor for the vector.
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id) , customers(customersList)
{

}

void OpenTable::act(Restaurant &restaurant)
{
	Table * curr_table = NULL;

	// test if the table_id is correct
	if (tableId < 0 || tableId >= restaurant.getNumOfTables() || 
		!(curr_table = restaurant.getTable(tableId)) ||
		((curr_table = restaurant.getTable(tableId))->isOpen()) || 
		customers.size() > curr_table->getCapacity())
	{
		cout << "Table does not exist or is already open" << endl;
		cout << "table id = " << tableId << " restaurant number of tables = " << restaurant.getNumOfTables() << " is open ? " <<curr_table->isOpen() << endl;
		cout << "amount of customers = " << customers.size() << " table capacity = " <<curr_table->getCapacity() <<  endl;
		return;
	}


	// open the table 
	curr_table->openTable();
	
	// add customers to table
	for (std::vector<Customer*>::const_iterator i = customers.begin(); i != customers.end(); ++i)
	{
		cout << "customer name " << (*i)->getName() << endl;
		curr_table->addCustomer(*i);
	}
}

std::string OpenTable::toString() const 
{

	return "test";
}

Order::Order(int id) : tableId(id) {}
void Order::act(Restaurant &restaurant) 
{
	

}
std::string Order::toString() const { return "test"; }
