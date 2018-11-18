#include "Customer.h"

// Customer parameterized constructor
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

// Customer copy constructor
Customer::Customer(const Customer &customer) : name(customer.name), id(customer.id) {}
// Customer move constructor
Customer::Customer(Customer &&customer) : name(customer.name), id(customer.id) {}
// copy assignment operator
Customer & Customer::operator=(const Customer &customer) {
	// to complete
}
// move assignment operator
Customer & Customer::operator=(Customer &&customer) {
	// to complete
}
// Destructor
Customer::~Customer() {}

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

std::string CheapCustomer::toString() const
{
	return this->getName() + ",chp ";
}

Customer* CheapCustomer::clone()
{
	CheapCustomer* cheap = new CheapCustomer(this->getName(), this->getId());
	cheap->ordered = this->ordered;

	return cheap;
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
		dishes_id_ordered_by_customer.push_back(veg_dish_id);
		dishes_id_ordered_by_customer.push_back(bvg_dish_id);
	}

	return dishes_id_ordered_by_customer;
}

std::string VegetarianCustomer::toString() const
{
	return this->getName() + ",veg ";
}

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
std::string SpicyCustomer::toString() const
{
	return this->getName() + ",spc ";
}

Customer* SpicyCustomer::clone()
{
	SpicyCustomer* spicy = new SpicyCustomer(this->getName(), this->getId());
	spicy->firstOrder = this->firstOrder;

	return spicy;
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

	// take the alc dish that wasn't order yet (doesn't exist in the dishes_id_ordered_by_customer)
	for (std::vector<Dish>::const_iterator i = vec_alc_dishes_after_sort.begin(); i != vec_alc_dishes_after_sort.end(); ++i)
	{
		std::vector<Dish>::iterator it;
		it = find(OrderedAlcDishes.begin(), OrderedAlcDishes.end(), *i);
		if (it != OrderedAlcDishes.end());
		else {
			// enter the new dish to the OrderedAlcDishes vector
			OrderedAlcDishes.push_back(*i);
			dishes_id_ordered_by_customer.push_back((*i).getId());
			break;
		}
	}

	return dishes_id_ordered_by_customer;
}
std::string AlchoholicCustomer::toString() const
{
	return this->getName() + ",alc ";
}

Customer* AlchoholicCustomer::clone()
{
	AlchoholicCustomer* alchoholic = new AlchoholicCustomer(this->getName(), this->getId());

	for (int i = 0; i < static_cast<int>(this->OrderedAlcDishes.size()); i++){
		alchoholic->OrderedAlcDishes.push_back(this->OrderedAlcDishes.at(i));
	}

	return alchoholic;
}

AlchoholicCustomer::~AlchoholicCustomer()
{
	OrderedAlcDishes.clear();
}