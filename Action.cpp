#include "Action.h"
#include "Restaurant.h"
#include <string>
#include <iostream>

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
	//system("pause");
}

std::string OpenTable::toString() const 
{

	return "test";
}




PrintMenu::PrintMenu() : BaseAction()
{

}


void PrintMenu::act(Restaurant &restaurant)
{
    vector<Dish> vDish;
    vDish = restaurant.getMenu();
    std::cout << "This is act by PrintMenu dervied class " << std::endl;
    for(int i = 0; i < vDish.size(); i++)
    {
        cout << vDish.at(i).getName() << " " << vDish.at(i).getType() << " " << vDish.at(i).getPrice() << "NIS" << endl;
    }
    //system("pause");
}

std::string PrintMenu::toString() const
{

    return "test";
}