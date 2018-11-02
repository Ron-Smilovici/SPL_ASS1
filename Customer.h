#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer {
public:
	Customer(std::string c_name, int c_id);
	virtual std::vector<int> order(const std::vector<Dish> &menu) = 0; // Derived class has to implement this
	virtual std::string toString() const = 0; // Derived class has to implement this
	std::string getName() const;
	int getId() const;
private:
	const std::string name;
	const int id;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
private:
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
private:
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
private:
};


#endif