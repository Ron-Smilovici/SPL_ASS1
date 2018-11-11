#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <sstream>
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"

using namespace std;

#define RESTAURANT_CLOSE 0
#define RESTAURANT_OPEN 1
#define DELIMITER_COMMA ','
#define DELIMITER_SPACE ' '

#define is_table_id_valid(t_id) !(t_id < 0 || t_id >= this->getNumOfTables())
#define is_table_open(t_id) (this->getTable(t_id))->isOpen()


//#define is_table_ok(t_id, is_open) !(t_id < 0 || t_id >= this->getNumOfTables() || \
	//									!(this->getTable(t_id)) || \
		//								((this->getTable(t_id))->isOpen()))

class Restaurant {
public:
	Restaurant();
	Restaurant(const std::string &configFilePath);
	Restaurant(const Restaurant &rhs); //copy constructor
	void start();
	int getNumOfTables() const; //
	void setOpen(bool value);
	Table* getTable(int ind);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
	std::vector<Dish>& getMenu();

private:
	bool open;
	std::vector<Table*> tables;
	std::vector<Dish> menu;
	std::vector<BaseAction*> actionsLog;

	/* private members */
	unsigned int number_of_tables;
	/* private methods */
	void parsingTables(string tables_capacity);
	void parsingDishes(string dish_information, int dish_id);
};

#endif