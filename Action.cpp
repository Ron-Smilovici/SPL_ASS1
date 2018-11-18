#include "Action.h"
#include "Restaurant.h" // Added because the use of all restaurant methods... check with someone else.
#include "Restaurant.h"
#include <string>
#include <iostream>

/* Forward declaration */
extern Restaurant* backup;

BaseAction::BaseAction()
{
	status = PENDING;
}

//copy constructor
BaseAction::BaseAction(const BaseAction &action)
{
	errorMsg = action.errorMsg;
	status = action.status;
}

BaseAction::~BaseAction() {

}

ActionStatus BaseAction::getStatus() const
{
	return this->status;
}

std::string BaseAction::getErrorMsg() const // by alon
{
	return this->errorMsg;
}

ostream& BaseAction::operator<<(ostream& out)
{
	return out << this->toString();
}

//error function
void BaseAction::error(std::string errorMsg) {
	status = ERROR;
	this->errorMsg = errorMsg;

	cout << "Error: " << errorMsg << endl;
}

void BaseAction::activate_error(std::string errorMsg)
{
	error(errorMsg);
}
void BaseAction::complete()
{
	this->status = COMPLETED;
}

// OpenTable action
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id), BaseAction()
{
	for (int i = 0; i < static_cast<int>(customersList.size()); ++i) {
		Customer* temp = customersList.at(i);
		Customer* cloned = temp->clone();
		customers.push_back(cloned);
	}
}

//copy constructor
OpenTable::OpenTable(const OpenTable &openTable) : tableId(openTable.tableId), BaseAction(openTable)
{
	for (int i = 0; i < static_cast<int>(openTable.customers.size()); ++i) {
		Customer* temp = openTable.customers.at(i);
		Customer* cloned = temp->clone();
		customers.push_back(cloned);
	}
}
//destructor
OpenTable::~OpenTable()
{
	for(Customer* pcsr : customers){
		delete(pcsr);
	}

	customers.clear();
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
		curr_table->addCustomer(*i);
	}

	this->complete();
}

std::string OpenTable::toString() const
{
	std::string str = "open " + std::to_string(this->tableId) + " ";
	for(int i = 0; i < static_cast<int>(this->customers.size()); i++){
		str.append(customers.at(i)->toString());
	}

	str += status_to_string();

	return str;
}

BaseAction* OpenTable::clone()
{
	return new OpenTable(*this);
}


// Order action
//parameterized constructor
Order::Order(int id) : BaseAction(), tableId(id) {}
//copy constructor (alon)
Order::Order(const Order &order) : tableId(order.tableId), BaseAction(order){}

void Order::act(Restaurant &restaurant)
{
	Table * curr_table;

	curr_table = restaurant.getTable(tableId);
	curr_table->order(restaurant.getMenu());

	this->complete();
}
std::string Order::toString() const
{
	return "order " + std::to_string(this->tableId) + " " + status_to_string();
}

BaseAction* Order::clone() //by alon
{
	return new Order(*this);
}


//MoveCustomer Action
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : BaseAction(), srcTable(src), dstTable(dst), id(customerId) {}
//copy constructor
MoveCustomer::MoveCustomer(const MoveCustomer &moveCustomer) : BaseAction(moveCustomer),
															   srcTable(moveCustomer.srcTable), dstTable(moveCustomer.dstTable), id(moveCustomer.id) {}

void MoveCustomer::act(Restaurant &restaurant)
{
	Customer * customter_to_move;
	Table * src_table, * dst_table;
	std::vector<OrderPair> orders_from_customer_to_move;

	src_table = restaurant.getTable(this->srcTable);
	customter_to_move = src_table->getCustomer(this->id);
	dst_table = restaurant.getTable(this->dstTable);

	// get orders from customer_to_move
	orders_from_customer_to_move = src_table->getCustomerOrders(customter_to_move->getId());
	// add orders to the dst_table
	dst_table->updateOrders(orders_from_customer_to_move);
	// add customer to dst table id
	dst_table->addCustomer(customter_to_move);
	// remove customer from src table id and the dishes he oredered on the src table
	src_table->removeCustomer(customter_to_move->getId());

	// check if src table needs to be closed due to no more customers
	if (!(src_table->getCustomers().size()))
		src_table->closeTable();

	this->complete();
}
std::string MoveCustomer::toString() const
{
	return "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " " + status_to_string();
}

BaseAction* MoveCustomer::clone()
{
	return new MoveCustomer(*this);
}


// Close action
Close::Close(int id) : BaseAction(), tableId(id) {}
//copy constructor
Close::Close(const Close &close) : tableId(close.tableId), BaseAction(close)
{
	bill = close.bill;
}

void Close::act(Restaurant &restaurant)
{
	Table * curr_table = NULL;

	curr_table = restaurant.getTable(tableId);
	if (!curr_table)
	{
		return;
	}

	this->bill = curr_table->getBill();

	// close the table
	curr_table->closeTable();

	std::stringstream sstr;
	sstr << "Table " << tableId << " was closed. Bill " << this->bill << "NIS" << endl;
	cout << sstr.str() << endl;

	this->complete();
}

std::string Close::toString() const
{
	return "close " + std::to_string(this->tableId) + " " + status_to_string() ;
}

BaseAction* Close::clone()
{
	return new Close(*this);
}


//CloseAll action
CloseAll::CloseAll() : BaseAction() {}
//copy constructor
CloseAll::CloseAll(const CloseAll &closeAll) : BaseAction(closeAll) {}

void CloseAll::act(Restaurant &restaurant)
{
	int number_of_tables = restaurant.getNumOfTables();

	for (int i = 0; i < number_of_tables; i++)
	{
		if (restaurant.getTable(i)->isOpen()) {
			Close close_table(i);
			close_table.act(restaurant);
			table_bills << close_table.toString();
		}
	}

	// close the restaurant
	restaurant.setOpen(false);

	this->complete();
}
std::string CloseAll::toString() const
{
	return table_bills.str();
}
BaseAction* CloseAll::clone()
{
	return new CloseAll(*this);
}


// PrintMenu action
PrintMenu::PrintMenu() : BaseAction() {}
//copy constructor
PrintMenu::PrintMenu(const PrintMenu &printMenu) : BaseAction(printMenu) {}

void PrintMenu::act(Restaurant &restaurant)
{
	vector<Dish> vDish;
	vDish = restaurant.getMenu();

	for (int i = 0; i < static_cast<int>(vDish.size()); i++)
	{
		cout << vDish.at(i).getName() << " " << convert_to_dishtype(vDish.at(i).getType())
			 << " " << vDish.at(i).getPrice() << "NIS" << endl;
	}
	this->complete();
}

std::string PrintMenu::toString() const { return "menu " + this->status_to_string(); }

BaseAction* PrintMenu::clone()
{
	return new PrintMenu(*this);
}


//PrintTableStatus action
PrintTableStatus::PrintTableStatus(int id) : BaseAction(), tableId(id) {}
//copy constructor
PrintTableStatus::PrintTableStatus(const PrintTableStatus &printTableStatus) : BaseAction(printTableStatus), tableId(printTableStatus.tableId)
{
	open = printTableStatus.open;
	bill = printTableStatus.bill;
	customers = printTableStatus.customers;
	dishes = printTableStatus.dishes;
}

void PrintTableStatus::act(Restaurant &restaurant)
{
	Table * curr_table;
	std::stringstream customer_string, dishes_string;
	std::stringstream sstr;
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

	
	sstr << "Table " << tableId << " status: " << open << endl;
	if (open == "open") {
		sstr << "Customers:" << endl;
		sstr << customers;
		sstr << "Orders:" << endl;
		sstr << dishes;
		sstr << "Current Bill: " << bill << "NIS";
		cout << sstr.str() << endl;
	}
	else
		cout << sstr.str();

	this->complete();
}

std::string PrintTableStatus::toString() const
{
	return "status " + to_string(this->tableId) + " " + this->status_to_string();
}

BaseAction* PrintTableStatus::clone()
{
	return new PrintTableStatus(*this);
}


//PrintActionsLog action
PrintActionsLog::PrintActionsLog() : BaseAction() {}
//copy constructor (alon)
PrintActionsLog::PrintActionsLog(const PrintActionsLog &printActionsLog) : BaseAction(printActionsLog) {}

void PrintActionsLog::act(Restaurant &restaurant)
{
	for(int i =0; i < static_cast<int>(restaurant.getActionsLog().size()); i++){
		cout << restaurant.getActionsLog().at(i)->toString() << endl;
	}

	this->complete();
}

std::string PrintActionsLog::toString() const { return "log " + this->status_to_string(); }

BaseAction* PrintActionsLog::clone()
{
	return new PrintActionsLog(*this);
}


//BackupRestaurant action
BackupRestaurant::BackupRestaurant() : BaseAction() {}
//copy constructor 
BackupRestaurant::BackupRestaurant(const BackupRestaurant &backupRestaurant) : BaseAction(backupRestaurant) {}

void BackupRestaurant::act(Restaurant &restaurant)
{
	if (!backup) {
		backup = new Restaurant(restaurant);
		this->complete();
		return;
	}

	*backup = restaurant;
	// update status to completed
	this->complete();
}

std::string BackupRestaurant::toString() const { return "backup " + this->status_to_string(); }

BaseAction* BackupRestaurant::clone()
{
	return new BackupRestaurant(*this);
}


//RestoreResturant action
RestoreResturant::RestoreResturant() : BaseAction() {}
//copy constructor 
RestoreResturant::RestoreResturant(const RestoreResturant &restoreResturant) : BaseAction(restoreResturant) {}

void RestoreResturant::act(Restaurant &restaurant)
{
	if(!backup){
		error("No backup available");
	}

	restaurant = *backup;

	this->complete();
}

std::string RestoreResturant::toString() const { return "restore " + this->status_to_string(); }

BaseAction* RestoreResturant::clone()
{
	return new RestoreResturant(*this);
}


/* Added methods */
std::string BaseAction::convert_to_dishtype(int num) {
	if (num == 0) return "VEG";
	else if (num == 1) return "SPC";
	else if (num == 2) return "BVG";
	else if (num == 3) return "ALC";
	else return "ERROR_DISH";
}

std::string BaseAction::status_to_string() const {

	if (this->getStatus() == PENDING) return "Pending";
	else if (this->getStatus() == COMPLETED) return "Completed";
	else return "Error: " + errorMsg;
}