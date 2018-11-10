#include <iostream>
#include <string>
#include "Dish.h"

//parameter constructor.
Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type) : id(d_id), name(d_name), price(d_price), type(d_type) {}

// copy constructor.
Dish::Dish(const Dish & rhs_lvalue): id(rhs_lvalue.id), name(rhs_lvalue.name), price(rhs_lvalue.price), type(rhs_lvalue.type) {}

// move constructor.
Dish::Dish(Dish && rhs_rvalue) : id(rhs_rvalue.id), name(rhs_rvalue.name), price(rhs_rvalue.price), type(rhs_rvalue.type) {}

//copy assignment operator.
Dish& Dish::operator=(const Dish& rhs){
    return *this;
}
//move assignment operator.
Dish& Dish::operator=( Dish&& rhs) {
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