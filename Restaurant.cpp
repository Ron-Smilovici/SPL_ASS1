#include "Restaurant.h"

/* Forward declaration */
DishType convert_to_dish(const std::string& str);
Actions convert_to_action(const std::string& str);
void split_str2vec(std::vector<string> * vec_str, std::string str);
void parse_customers(std::vector<string> argv, std::vector<Customer *> & customers);

void erase_op_code(std::vector<string> & vec);
int extract_id(std::vector<string> & vec);

// TO BE DELETED!
void print_vector(std::vector<Table*> vec_tables)
{
	cout << "Printing Vector" << endl;
	for (std::vector<Table*>::const_iterator i = vec_tables.begin(); i != vec_tables.end(); ++i)
		cout << (*i)->getCapacity() << endl;  // (*i) meanning the first element inside the vector.
}

void print_vector_string(std::vector<string> vec_tables)
{
	cout << "Printing Vector" << endl;
	for (std::vector<string>::const_iterator i = vec_tables.begin(); i != vec_tables.end(); ++i)
		cout << *i << endl;
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

	while (getline(ss, capacity, DELIMITER_COMMA))
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

	getline(ss, name, DELIMITER_COMMA);
	getline(ss, type, DELIMITER_COMMA);
	getline(ss, price, DELIMITER_COMMA);

	// Create a Dish
	Dish dish(dish_id, name, stoul(price), convert_to_dish(type));

	// Advance Dish id
	dish_id++;

	// Add the Dish to the menu vector
	menu.push_back(dish);
	//cout << "name = " << name << " type= " << type << " price = " << price << endl;
}

void Restaurant::start()
{
	bool finish = false;
	string user_input;
	Actions op_code;
	size_t pos = 0;

	vector<Customer *> vec_customers;

	open = true;
	std::cout << "Restaurant is now open!" << std::endl;
	
	while (getline(cin, user_input) && !finish) {
		vector<string> argv;
		cout << "for test: user_input= " << user_input << endl;

		// Split user command to words in vector
		split_str2vec(&argv, user_input);

		print_vector_string(argv); // for test

		op_code = convert_to_action(argv.at(0));
		switch (op_code)
		{
			case OPEN:
				int table_id;
				// Deletes the op_code element from vector argv[0]
				erase_op_code(argv);

				// fill customer vector
				table_id = extract_id(argv);
				parse_customers(argv, vec_customers);
	
				BaseAction *ba = new OpenTable(table_id, vec_customers);
				ba->act(*this);

				cout << "open command" << endl;
			break;
			/*case ORDER:
				cout << "order command" << endl;
				BaseAction *ba = new OpenTable(table_id, vec_customers);
			break;
			case MOVE:
				cout << "move command" << endl;
				BaseAction *ba = new OpenTable(table_id, vec_customers);
			break;
			case CLOSEALL:
				cout << "closeall command" << endl;
				BaseAction *ba = new OpenTable(table_id, vec_customers);
				finish = true;
			break;



		default:
			break;*/
		}
		
	}
	cout << "finishing commands" << endl;
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
DishType convert_to_dish(const std::string& str)
{
	if (str == "VEG") return VEG;
	else if (str == "SPC") return SPC;
	else if (str == "BVG") return BVG;
	else if (str == "ALC") return ALC;

}

Actions convert_to_action(const std::string& str)
{
	//OPEN, ORDER, MOVE, CLOSE, CLOSEALL, MENU, STATUS, LOG, BACKUP, RESTORE
	if (str == "open") return OPEN;
	else if (str == "order") return ORDER;
	else if (str == "move") return MOVE;
	else if (str == "close") return CLOSE;
	else if (str == "closeall") return CLOSEALL;

}

CustomerType convert_to_customer(const std::string& str) 
{
	if (str == "veg") return veg;
	else if (str == "chp") return chp;
	else if (str == "spc") return spc;
	else if (str == "alc") return alc;
}

void split_str2vec (std::vector<string> * vec_str, std::string str) 
{
	std::string token;
	std::istringstream ss(str);
	while (std::getline(ss, token, ' ')) {
		vec_str->push_back(token);
	}
}


/* Open Action */
void erase_op_code(std::vector<string> & vec) 
{
	vec.erase(vec.begin() + 0); 
}

int extract_id(std::vector<string> & vec) 
{
	int id = stoi(vec.at(0), nullptr, 10);
	vec.erase(vec.begin() + 0);
	return id;
}

void parse_customers(std::vector<string> argv, std::vector<Customer *> & vec_customers)
{
	Customer * customer;
	int customer_id = 0;

	for (std::vector<string>::const_iterator i = argv.begin(); i != argv.end(); ++i) {
		stringstream ss(*i);

		//std::string customer_name = *i;
		//CustomerType customer_type = convert_to_customer(*(i++));
		std::string customer_name, customer_str_type;
		getline(ss, customer_name, ',');
		getline(ss, customer_str_type, ',');
		CustomerType customer_type = convert_to_customer(customer_str_type);

		cout << "customer name = " << customer_name << "  customer type = " << customer_type << endl;
		switch (customer_type)
		{
		case veg:
			customer = new VegetarianCustomer(customer_name, customer_id);
			break;
		case chp:
			customer = new CheapCustomer(customer_name, customer_id);
			break;
		case spc:
			customer = new SpicyCustomer(customer_name, customer_id);
			break;
		case alc:
			customer = new AlchoholicCustomer(customer_name, customer_id);
			break;
		}
		vec_customers.push_back(customer);
		customer_id++;
	}
}


/* Other Actions */