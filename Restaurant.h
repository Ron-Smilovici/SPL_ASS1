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

class Restaurant {
public:
	Restaurant();
	Restaurant(const std::string &configFilePath);
	void start();
	int getNumOfTables() const; //
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
	void parsingDishes(string dish_information);
};

#endif