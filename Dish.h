#ifndef DISH_H_
#define DISH_H_

#include <string>

enum DishType {
	VEG, SPC, BVG, ALC, ERROR_DISH
};

class Dish {
public:
	Dish(int d_id, std::string d_name, int d_price, DishType d_type);
	Dish(const Dish& rhs_lvalue);// copy constructor.
    Dish(Dish&& rhs_rvalue);// move constructor.

    int getId() const;
	std::string getName() const;
	int getPrice() const;
	DishType getType() const;
	Dish& operator=(const Dish& rhs_lvalue); //copy assignment operator.
    Dish& operator=(Dish&& rhs_rvalue); //move assignment operator.

private:
	const int id;
    const std::string name;
    const int price;
	const DishType type;
};


#endif