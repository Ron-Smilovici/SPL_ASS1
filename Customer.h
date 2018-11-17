#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include <string>
#include <iostream>
#include <algorithm>
#include <limits.h>
#include "Dish.h"

using namespace std;

enum CustomerType {
	veg, spc, chp, alc, err
};

#define MAX INT_MAX
#define MIN INT_MIN

typedef std::pair<int, int> CustomerOrderPair;

class Customer {
public:
	Customer(std::string c_name, int c_id);
	Customer(const Customer &customer); //copy constructor
	Customer(Customer &&customer); //move constructor
	Customer& operator=(const Customer & customer); //copy assignment operator
	Customer& operator=(Customer && customer); //move assignment operator
	virtual ~Customer(); //Destructor
	virtual Customer* clone() = 0; // create a dynamic copy of the specific child customer
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
	Customer* clone(); // create a dynamic copy of the specific child customer
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	Customer* clone(); // create a dynamic copy of the specific child customer
private:
	bool ordered;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	Customer* clone(); // create a dynamic copy of the specific child customer
private:
	bool firstOrder;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	Customer* clone(); // create a dynamic copy of the specific child customer
	~AlchoholicCustomer(); //Destructor
private:
	std::vector<Dish> OrderedAlcDishes;
};


#endif