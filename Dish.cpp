#include "Dish.h"
#include <iostream>
#include <string>

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price), type(d_type) {}

// copy constructor
Dish::Dish(const Dish & dish) : id(dish.id), name(dish.name), price(dish.price), type(dish.type) {}

// move constructor
Dish::Dish(Dish && dish) : id(dish.id), name(dish.name), price(dish.price), type(dish.type) {}

// copy assignement operator
Dish & Dish::operator=(const Dish & dish) {
	return *this;
}

// move assignement operator
Dish & Dish::operator=(Dish && dish) {
	return *this;
}

int Dish::getId() const
{
	return this->id;
}

std::string Dish::getName() const
{
	return this->name;
}

int Dish::getPrice() const
{
	return this->price;
}

DishType Dish::getType() const
{
	return this->type;
}

bool Dish::operator==(const Dish& dish) const 
{
	return ((this->getId() == dish.getId()) &&
		(this->getName() == dish.getName()) &&
		(this->getPrice() == dish.getPrice()) &&
		(this->getType() == dish.getType()));
}

bool Dish::operator< (Dish& dish) const
{
	cout << "Test this->price= " << this->price << " < " << dish.getPrice() << " =dish.getPrice()" << endl;
	return (this->price < dish.getPrice());
}