#include "Restaurant.h"

/* Forward declaration */
DishType convert(const std::string& str);

// TO BE DELETED!
void print_vector(std::vector<Table*> vec_tables)
{
	cout << "Printing Vector" << endl;
	for (std::vector<Table*>::const_iterator i = vec_tables.begin(); i != vec_tables.end(); ++i)
		cout << (*i)->getCapacity() << endl;
}
//////////////


//Restaurant();
Restaurant::Restaurant(const std::string &configFilePath) : number_of_tables(0)
{
	bool r_num_of_tables_complete = false;
	bool r_tables_complete = false;
	bool r_menu_complete = false;
	ifstream input_file(configFilePath);

	if (input_file.is_open()) 
	{
		string line;
		while (getline(input_file, line)) 
		{
			/* ignore line starts with '#' or empty line */
			if (line[0] == '#' || line.empty())
				continue;

			if (!r_num_of_tables_complete) {
				number_of_tables = stoul(line);
				r_num_of_tables_complete = true;
				continue;
			}

			if (!r_tables_complete) {
				parsingTables(line);
				r_tables_complete = true;
				continue;
			}
			cout << line << endl;
			parsingDishes(line);
		}
		cout << "Finishing parsing configuration file" << endl;
	}
	else {
		std::cerr << "Couldn't open config file " << configFilePath << " for reading.\n";
	}
}

void Restaurant::parsingTables(string tables_capacity) 
{
	string capacity;
	stringstream ss(tables_capacity);

	while (getline(ss, capacity, DELIMITER))
	{
		// create a new table with certain capacity
		Table * table = new Table(stoul(capacity));

		// insert table to the vector of tables
		tables.push_back(table);
	}
}

void Restaurant::parsingDishes(string dish_information)
{
	string name, type, price;
	stringstream ss(dish_information);
	static int dish_id = 0;

	getline(ss, name, DELIMITER);
	getline(ss, type, DELIMITER);
	getline(ss, price, DELIMITER);

	// Create a Dish
	Dish dish(dish_id, name, stoul(price), convert(type));

	// Advance Dish id
	dish_id++;

	// Add the Dish to the menu vector
	menu.push_back(dish);
	cout << "name = " << name << " type= " << type << " price = " << price << endl;
}

void Restaurant::start()
{
	string user_input;
	open = true;
	std::cout << "Restaurant is now open!" << std::endl;
	
	while (cin >> user_input) {
	
	
	}
}


int Restaurant::getNumOfTables() const
{
	return tables.size();
}

Table* Restaurant::getTable(int ind) 
{
	// ADD test on ind fo the table
	return tables.at(ind);
}

// Return a reference to the history of actions
//const std::vector<BaseAction*>& Restaurant::getActionsLog() const{}
//std::vector<Dish>& Restaurant::getMenu() {}





/* Added methods */
DishType convert(const std::string& str)
{
	if (str == "VEG") return VEG;
	else if (str == "SPC") return SPC;
	else if (str == "BVG") return BVG;
	else if (str == "ALC") return ALC;
}

