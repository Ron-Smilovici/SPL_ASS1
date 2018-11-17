#include "Customer.h"

// Customer parameterize constructor
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

// Customer copy constructor
Customer::Customer(const Customer &customer) : name(customer.name), id(customer.id) {}
// Customer move constructor
Customer::Customer(Customer &&customer) : name(customer.name), id(customer.id) {}
// copy assignment operator
Customer & Customer::operator=(const Customer &customer) { }
// move assignment operator
Customer & Customer::operator=(Customer &&customer) {}
// Destructor
Customer::~Customer() 
{
	cout << "Destructor Customer was called" << endl;
}
std::string Customer::getName() const { return name; }
int Customer::getId() const { return id; }

// CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), ordered(false) {}
//returns a vector of dishes IDs that were ordered by the customers
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) 
{ 
	std::vector<CustomerOrderPair> temp_vec_dish;
	std::vector<int> dishes_id_ordered_by_customer;
	int lowest_dish_price;
	int dish_id;

	if (this->ordered) {
		//cout << "CheapCustomer already ordered, return an empty vector" << endl; // will be deleted.
		return dishes_id_ordered_by_customer;
	}

	lowest_dish_price = menu.at(0).getPrice();
	dish_id = 0;

	for (std::vector<Dish>::const_iterator i = menu.begin() + 1; i != menu.end(); ++i)
	{
		if ((*i).getPrice() < lowest_dish_price) {
			dish_id = (*i).getId();
			lowest_dish_price = (*i).getPrice();
		}
	}

	dishes_id_ordered_by_customer.push_back(dish_id);

	// the CheapCustomer has now ordered
	this->ordered = true;

	return dishes_id_ordered_by_customer;
}


std::string CheapCustomer::toString() const { return "toString of CheapCustomer\n"; }

Customer* CheapCustomer::clone()
{
	return new CheapCustomer(this->getName(), this->getId());
}

// VegetarianCustomer
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(name, id) {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) 
{
	std::vector<int> dishes_id_ordered_by_customer;
	int bvg_dish_id = -1, veg_dish_id = -1;
	bool found_veg = false;
	bool found_bvg = false;
	int bvg_dish_price = MIN;
	//cout << " VegetarianCustomer::order starts" << endl;
	for (std::vector<Dish>::const_iterator i = menu.begin(); i != menu.end(); ++i)
	{
		// orders the veg dish with smallest id
		if (!found_veg && ((*i).getType() == VEG))
		{
			veg_dish_id = (*i).getId();
			found_veg = true;
		}

		// orders the most expensive beverage(Non - alcoholic)
		if (((*i).getType() == BVG) && ((*i).getPrice() > bvg_dish_price))
		{
			bvg_dish_id = (*i).getId();
			bvg_dish_price = (*i).getPrice();
		}
	}
	if (bvg_dish_id != -1)
		found_bvg = true;

	if (found_veg && found_bvg)
	{
		//cout << " VegetarianCustomer::order found veg" << endl;
		//cout << " VegetarianCustomer::order found bvg" << endl;
		dishes_id_ordered_by_customer.push_back(veg_dish_id);
		dishes_id_ordered_by_customer.push_back(bvg_dish_id);
	}

	return dishes_id_ordered_by_customer;
}

std::string VegetarianCustomer::toString() const { return "test"; }

Customer* VegetarianCustomer::clone()
{
	return new VegetarianCustomer(this->getName(), this->getId());
}

// SpicyCustomer
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(name, id) , firstOrder(true) {}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) 
{
	std::vector<int> dishes_id_ordered_by_customer;
	int dish_price = MIN;
	int dish_id = -1;
	bool spc_dish_exist = false;

	if (firstOrder)
	{
		for (std::vector<Dish>::const_iterator i = menu.begin(); i != menu.end(); ++i)
		{
			// orders the most expensive spicy dish in the menu
			if (((*i).getType() == SPC) && ((*i).getPrice() > dish_price))
			{
				dish_price = (*i).getPrice();
				dish_id = (*i).getId();

				// spc order exist in the menu
				spc_dish_exist = true;
			}

		}

		// if there isn't a spc dish in the menu customer will not order
		if (!spc_dish_exist) {
			//cout << "there is no spc dish in the menu, customer will not orderd" << endl;
			return dishes_id_ordered_by_customer;
		}

		dishes_id_ordered_by_customer.push_back(dish_id);

		// get ready for the next order
		firstOrder = false;
	}
	else  // the customer already ordered a spc dish he will now order the cheapest non-alcoholic beverage
	{

		dish_price = MAX;
		for (std::vector<Dish>::const_iterator i = menu.begin(); i != menu.end(); ++i)
		{
			// orders the most expensive spicy dish in the menu
			if (((*i).getType() == BVG) && ((*i).getPrice() < dish_price))
			{
				dish_price = (*i).getPrice();
				dish_id = (*i).getId();
			}
		}

		dishes_id_ordered_by_customer.push_back(dish_id);
	}
	return dishes_id_ordered_by_customer;
}
std::string SpicyCustomer::toString() const { return "test SpicyCustomer"; }

Customer* SpicyCustomer::clone()
{
	return new SpicyCustomer(this->getName(), this->getId());
}


// AlchoholicCustomer
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(name,id) {}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) 
{ 
	std::vector<Dish> vec_alc_dishes_before_sort;
	std::vector<Dish> vec_alc_dishes_after_sort;
	std::vector<int> dishes_id_ordered_by_customer;
	std::vector<std::pair<int,int>> dish_prices_id;
	
	// Copy all alchoholic dishes from menu to a temp vector
	for (std::vector<Dish>::const_iterator i = menu.begin(); i != menu.end(); ++i)
		if ((*i).getType() == ALC) {
			vec_alc_dishes_before_sort.push_back(*i);
			dish_prices_id.push_back(std::make_pair((*i).getPrice(),(*i).getId()));
		}
	
	/*cout << "alchoholic dishes before sort:" << endl;
	for (std::vector<Dish>::const_iterator i = vec_alc_dishes_before_sort.begin(); i != vec_alc_dishes_before_sort.end(); ++i)
	{ cout << "Dish: Name " << (*i).getName() << " id " << (*i).getId() << " price " << (*i).getPrice() << endl;}*/
	
	// Using simple sort() function to sort the alc dishes by price
	sort(dish_prices_id.begin(), dish_prices_id.end());
	for (std::vector< std::pair<int, int>>::const_iterator i = dish_prices_id.begin(); i != dish_prices_id.end(); ++i)
	{
		for (std::vector<Dish>::const_iterator j = vec_alc_dishes_before_sort.begin(); j != vec_alc_dishes_before_sort.end(); ++j)
	{
			if ((*j).getId() == (*i).second) {
				vec_alc_dishes_after_sort.push_back(*j);
				break;
			}
	}
	
	}

	/*cout << "alchoholic dishes after sort:" << endl;
	for (std::vector<Dish>::const_iterator j = vec_alc_dishes_after_sort.begin(); j != vec_alc_dishes_after_sort.end(); ++j)
	{ cout << "Dish: Name " << (*j).getName() << " id " << (*j).getId() << " price " << (*j).getPrice() << endl;}*/
	
	// take the alc dish that wasn't order yet (doesn't exist in the dishes_id_ordered_by_customer)
	for (std::vector<Dish>::const_iterator i = vec_alc_dishes_after_sort.begin(); i != vec_alc_dishes_after_sort.end(); ++i)
	{
		std::vector<Dish>::iterator it;
		it = find(OrderedAlcDishes.begin(), OrderedAlcDishes.end(), *i);
		if (it != OrderedAlcDishes.end())
			;// std::cout << "Dish " << (*i).getName() << " already oredered continue to the next one" << endl;
		else {
			//std::cout << "Dish " << (*i).getName() << " wasn't oreder before, order it now" << endl;
			// enter the new dish to the OrderedAlcDishes vector
			OrderedAlcDishes.push_back(*i);
			dishes_id_ordered_by_customer.push_back((*i).getId());
			break;
		}
	}

	return dishes_id_ordered_by_customer;
}
std::string AlchoholicCustomer::toString() const { return "test AlchoholicCustomer"; }

Customer* AlchoholicCustomer::clone()
{
	return new AlchoholicCustomer(this->getName(), this->getId());
}

AlchoholicCustomer::~AlchoholicCustomer() 
{
	cout << "Destructor AlchoholicCustomer called" << endl;
	OrderedAlcDishes.clear();
}