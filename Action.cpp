#include "Action.h"
#include "Restaurant.h" // Added becuase the use of all restaurant methods... check with someone else.
#include "Restaurant.h"
#include <string>
#include <iostream>

/* Forward declaration */
extern Restaurant* backup;
std::string convert_to_dishtype(int num);

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

void OpenTable::act(Restaurant &restaurant)
{
	Table * curr_table = NULL;

	curr_table = restaurant.getTable(tableId);
	// open the table 
	curr_table->openTable();
	// add customers to table
	for (std::vector<Customer*>::const_iterator i = customers.begin(); i != customers.end(); ++i)
	{
		cout << "customer name " << (*i)->getName() << " id " << (*i)->getId() << " will be added to table " << tableId << endl;
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

	curr_table = restaurant.getTable(tableId);
	curr_table->order(restaurant.getMenu());
}

std::string Order::toString() const { return "test"; }

// Move Action
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}

/*Moves a customer from one table to another. Also moves all orders
made by this customer from the bill of the origin table to the bill of the destination table.
If the origin table has no customers left after this move, the program will close the origin
table. */
void MoveCustomer::act(Restaurant &restaurant) 
{
	Customer * customter_to_move;
	Table * src_table, * dst_table;
	std::vector<OrderPair> orders_from_customer_to_move;
	src_table = restaurant.getTable(this->srcTable);
	customter_to_move = src_table->getCustomer(this->id);
	dst_table = restaurant.getTable(this->dstTable);

	// get orders from customter_to_move
	orders_from_customer_to_move = src_table->getCustomerOrders(customter_to_move->getId());
	// add orders to the dst_table
	dst_table->updateOrders(orders_from_customer_to_move);
	// add customer to dst table id
	dst_table->addCustomer(customter_to_move);

	// remove customer from src table id and the dishes he oredered on the src table
	src_table->removeCustomer(customter_to_move->getId());


	// check if src table needs to be closed due to no more customers



}
std::string MoveCustomer::toString() const { return "toString of MoveCustomer "; }

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

// PrintMenu action

PrintMenu::PrintMenu() : BaseAction() {}
void PrintMenu::act(Restaurant &restaurant)
{
	vector<Dish> vDish;
	vDish = restaurant.getMenu();
	std::cout << "This is act by PrintMenu dervied class " << std::endl;
	for (int i = 0; i < vDish.size(); i++)  // printing.
	{
		cout << vDish.at(i).getName() << " " << convert_to_dishtype(vDish.at(i).getType())
			<< " " << vDish.at(i).getPrice() << "NIS" << endl;
	}
	//system("pause");
}

std::string PrintMenu::toString() const
{
	return "test";
}

//PrintTableStatus action
PrintTableStatus::PrintTableStatus(int id) :BaseAction(), tableId(id) {}

void PrintTableStatus::act(Restaurant &restaurant) 
{
	Table * curr_table;
	std::stringstream customer_string, dishes_string;

	curr_table = restaurant.getTable(tableId);
	if (curr_table->isOpen())
	{
		open = "open";
		for (std::vector<Customer*>::const_iterator i = curr_table->getCustomers().begin();
				i != curr_table->getCustomers().end();
				++i)
			customer_string << (*i)->getId() << " " << (*i)->getName() << endl;

		customers = customer_string.str();

		for (std::vector<OrderPair>::const_iterator i = curr_table->getOrders().begin();
				i != curr_table->getOrders().end();
				++i)
			dishes_string << (*i).second.getName() << " " << (*i).second.getPrice() << "NIS " << (*i).first << endl;
		
		dishes = dishes_string.str();
		bill = curr_table->getBill();
	}
	else {
		open = "closed";
	}
}

std::string PrintTableStatus::toString() const 
{
	std::stringstream sstr;
	sstr << "Table " << tableId << " status: " << open << endl;
	if (open == "open") {
		sstr << "Customers:" << endl;
		sstr << customers;
		sstr << "Orders:" << endl;
		sstr << dishes << endl;
		sstr << "Current Bill: " << bill << "NIS" << endl;
	}
	return sstr.str();
}

//BackupRestaurant action

BackupRestaurant::BackupRestaurant() : BaseAction() {}
void BackupRestaurant::act(Restaurant &restaurant)
{
	std::cout << "This is act by BackupRestaurant dervied class " << std::endl;
	if (!backup) {
		cout << "backup is empty." << endl;
		backup = new Restaurant(restaurant);
	}

	//backup = Restaurant
}

std::string BackupRestaurant::toString() const { return "test"; }

/* Added methods */
std::string convert_to_dishtype(int num) {
	if (num == 0) return "VEG";
	else if (num == 1) return "SPC";
	else if (num == 2) return "BVG";
	else if (num == 3) return "ALC";
	else return "ERROR_DISH";
}