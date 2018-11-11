#include "Action.h"
#include "Restaurant.h" // Added becuase the use of all restaurant methods... check with someone else.
#include "Restaurant.h"
#include <string>
#include <iostream>

BaseAction::BaseAction() 
{

}

ActionStatus BaseAction::getStatus() const 
{
	return this->status;
}

ostream& BaseAction::operator<<(ostream& out)
{
	return out << this->toString();
}

// make sure regarding the copy constuctor for the vector.
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id) , customers(customersList)
{

}

PrintMenu::PrintMenu() : BaseAction()
{
}
void PrintMenu::act(Restaurant &restaurant)
{
	vector<Dish> vDish;
	vDish = restaurant.getMenu();
	std::cout << "This is act by PrintMenu dervied class " << std::endl;
	for (int i = 0; i < vDish.size(); i++)
	{
		cout << vDish.at(i).getName() << " " << vDish.at(i).getType() << " " << vDish.at(i).getPrice() << "NIS" << endl;
	}
	//system("pause");
}
std::string PrintMenu::toString() const
{
	return "test";
}

void OpenTable::act(Restaurant &restaurant)
{
	Table * curr_table = NULL;

	curr_table = restaurant.getTable(tableId);
	// open the table 
	curr_table->openTable();
	// add customers to table
	for (std::vector<Customer*>::const_iterator i = customers.begin(); i != customers.end(); ++i)
	{
		cout << "customer name " << (*i)->getName() << " will be added to table " << tableId << endl;
		curr_table->addCustomer(*i);
	}
}

std::string OpenTable::toString() const 
{
	return "";
}

Order::Order(int id) : BaseAction(), tableId(id) {}

void Order::act(Restaurant &restaurant) 
{
	Table * curr_table;

	if (!(curr_table = restaurant.getTable(tableId)))
	{
		cout << "Need to add error could not find table" << endl;
	}

	curr_table->order(restaurant.getMenu());
}
std::string Order::toString() const { return "test"; }



// Close action
Close::Close(int id) : BaseAction(), tableId(id) {}

void Close::act(Restaurant &restaurant)
{
	Table * curr_table = NULL;

	curr_table = restaurant.getTable(tableId);
	if (!curr_table)
	{
		cout << "Close::act curr_table is NULL" << endl;
		return;
	}

	this->bill = curr_table->getBill();

	// close the table
	curr_table->closeTable();
}

std::string Close::toString() const 
{
	std::stringstream sstr;
	sstr << "Table " << tableId << " was closed. Bill " << this->bill << endl;
	//sstr << "Address: houseNumber: " << houseNumber << " streetName: " << streetName << " zipCode: " << zipCode;
	return sstr.str(); 
}

//CloseAll action

CloseAll::CloseAll() : BaseAction() {}
void CloseAll::act(Restaurant &restaurant)
{
	int number_of_tables = restaurant.getNumOfTables();

	for (int i = 0; i < number_of_tables; i++)
	{
		if (restaurant.getTable(i)->isOpen()) {
			Close close_table(i);
			close_table.act(restaurant);
			cout << close_table.toString();
		}
	}

	// close the restaurant
	restaurant.setOpen(false);
}
std::string CloseAll::toString() const { return ""; }
