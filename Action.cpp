#include "Action.h"

BaseAction::BaseAction() 
{

}

// make sure regarding the copy constuctor for the vector.
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : BaseAction(), tableId(id) , customers(customersList)
{

}

void OpenTable::act(Restaurant &restaurant) 
{
	std::cout << "This is act by OpenTable dervied class " << std::endl;
	system("pause");
}

std::string OpenTable::toString() const 
{

	return "test";
}