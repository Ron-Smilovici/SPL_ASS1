#include "Customer.h"

// Customer
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id)
{

}

std::string Customer::getName() const { return name; }
int Customer::getId() const { return id; }

// CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {}

//returns a vector of dishes IDs that were ordered by the customers
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu)
{
	int cheapest_dish_id = 0;
	int cheapest_dish_price = menu.at(0).getPrice();
	vector<Dish> temp_vec_dish;
	vector<int> temp;
	// find the cheapest dish in the menu
	for (std::vector<Dish>::const_iterator i = menu.begin() + 1; i != menu.end(); ++i)
	{
		if ((*i).getPrice() < cheapest_dish_price)
		{
			cheapest_dish_price = (*i).getPrice();
			cheapest_dish_id = (*i).getId();
		}
	}

	cout << "cheapest dish is " << menu.at(cheapest_dish_id).getName() << " price " <<
		 (menu.at(cheapest_dish_id)).getPrice() << " id " << cheapest_dish_id << endl;

	// remove the dish from menu
	cout << "remove the dish from menu" << endl;
	for (std::vector<Dish>::const_iterator i = menu.begin(); i != menu.end(); ++i)
	{
		if ((*i).getId() == cheapest_dish_id)
			continue;
		temp_vec_dish.push_back(*i);
	}

	//menu.clear();

	// take all dishes from temp vec to menu
	cout << "update the dish menu without the dish that was ordered" << endl;
	for (std::vector<Dish>::const_iterator i = temp_vec_dish.begin(); i != temp_vec_dish.end(); ++i)
	{
		//menu.push_back(*i);
	}

	//return vector<int> te
	return temp;
}
std::string CheapCustomer::toString() const { return "test"; }

// VegetarianCustomer
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {}
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string VegetarianCustomer::toString() const { return "test"; }

// SpicyCustomer
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) {}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string SpicyCustomer::toString() const { return "test"; }

// AlchoholicCustomer
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name,id) {}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) { std::vector<int> vec; return vec; }
std::string AlchoholicCustomer::toString() const { return "test"; }