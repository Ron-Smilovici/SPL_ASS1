#include "Restaurant.h"

/* Forward declaration */
DishType convert_to_dish(const std::string& str);
Actions convert_to_action(const std::string& str);
void split_str2vec(std::vector<string> * vec_str, std::string str);
void create_customers(std::vector<string> argv, std::vector<Customer *> & customers);

void erase_op_code(std::vector<string> & vec);
int extract_table_id(std::vector<string> & vec);

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
	int dish_id = 0;

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

			parsingDishes(line, dish_id);

			// Advance Dish id
			dish_id++;
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
		Table * table = new Table(stoul(capacity));
		// create a new table with certain capacity
		cout << "create a NEW table with capacity " << stoul(capacity) << "table is_open? " << table->isOpen() << endl;

		// insert table to the vector of tables. push_back adds a new value to the end of the array(vector).
		tables.push_back(table);
	}
}

void Restaurant::parsingDishes(string dish_information, int dish_id)
{
	string name, type, price;
	stringstream ss(dish_information);

	getline(ss, name, DELIMITER_COMMA);
	getline(ss, type, DELIMITER_COMMA);
	getline(ss, price, DELIMITER_COMMA);

	// Create a Dish on the STK. coping it to the vector of Dishes.
	Dish dish(dish_id, name, stoul(price), convert_to_dish(type));

	// Add the Dish to the menu vector
	menu.push_back(dish);
//	cout << "name = " << name << " type= " << type << " price = " << price << endl;
//	cout << "x" << menu.at(0).getName() << endl;
}

void Restaurant::start()
{
	static string rname = "Restaurant::start ";
	bool finish = false;
	string user_input;
	Actions op_code;
	int table_id;
	BaseAction * ba = NULL;

	// open the restaurant
	open = true;
	std::cout << "Restaurant is now open!" << std::endl;

	while (getline(cin, user_input) && !finish) {
		vector<string> argv;
		vector<Customer *> vec_customers;

		//cout << "for test: user_input= " << user_input << endl;

		// Split user command to words in vector
		split_str2vec(&argv, user_input);

		//print_vector_string(argv); // for test

		op_code = convert_to_action(argv.at(0));

		// Delete the op_code element from vector argv[0]
		erase_op_code(argv);

		switch (op_code)
		{
			case OPEN:
				cout << "open command" << endl;

				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is already open, this action should result in an error */
				if (!is_table_id_valid(table_id) || is_table_open(table_id)) {
					cout << "Table does not exist or is already open" << endl;
					cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					continue;
				}

				/* check the cpacity of the table */
				if (argv.size() > (this->getTable(table_id))->getCapacity())
				{
					cout << "requsted table for " << argv.size() << " persons. Table capacity = " << this->getTable(table_id)->getCapacity() << endl;
					continue;
				}

				// fill customer vector
				create_customers(argv, vec_customers);
				ba = new OpenTable(table_id, vec_customers);
				
			break;
			case ORDER:
				cout << "order command" << endl;
				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is closed, this action should result in an error */
				if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
					cout << "Table does not exist or is already open" << endl;
					cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					continue;
				}

				ba = new Order(table_id);
			break;
			/*case MOVE:
				cout << "move command" << endl;
				BaseAction *ba = new OpenTable(table_id, vec_customers);
			break;
			case CLOSEALL:
				cout << "closeall command" << endl;
				BaseAction *ba = new OpenTable(table_id, vec_customers);
				finish = true;
			break;
			*/
			case CLOSE:
				cout << "close command" << endl;
				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is already closed, this action should result in an error */
				if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
					cout << "Table does not exist or is already open" << endl;
					cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					cout << "table is_open?" << this->getTable(table_id)->isOpen() << endl;
					continue;
				}

				ba = new Close(table_id);
				break;

			case CLOSEALL:
				cout << "closeall command" << endl;
				ba = new CloseAll();
				break;

		    case MENU:
		        cout << "menu commend" << endl;
		        BaseAction *ba = new PrintMenu();
		        PrintMenu printMenu;
		        printMenu.act(*this);
                break;

		/*default:
			break;*/
		}

		ba->act(*this);
		cout << ba->toString();

		// log the action in the log
		actionsLog.push_back(ba);
		cout << endl;
	}
	cout << "finishing commands" << endl;
}


int Restaurant::getNumOfTables() const
{
	//return tables.size();
	return number_of_tables;
}

Table* Restaurant::getTable(int ind) 
{
	return tables.at(ind);
}


void Restaurant::setOpen(bool value) {
		this->open = value;
	}

// Return a reference to the history of actions
//const std::vector<BaseAction*>& Restaurant::getActionsLog() const{}

	std::vector<Dish> &Restaurant::getMenu() {
		return this->menu;
	}


/* Added methods */
	DishType convert_to_dish(const std::string &str) {
		if (str == "VEG") return VEG;
		else if (str == "SPC") return SPC;
		else if (str == "BVG") return BVG;
		else if (str == "ALC") return ALC;
		else return ERROR_DISH;
	}

	Actions convert_to_action(const std::string &str) {
		//OPEN, ORDER, MOVE, CLOSE, CLOSEALL, MENU, STATUS, LOG, BACKUP, RESTORE
		if (str == "open") return OPEN;
		else if (str == "order") return ORDER;
		else if (str == "move") return MOVE;
		else if (str == "close") return CLOSE;
		else if (str == "closeall") return CLOSEALL;

		else if (str == "menu") return MENU;

		else return ERROR_ACTION;
	}

	CustomerType convert_to_customer(const std::string &str) {
		if (str == "veg") return veg;
		else if (str == "chp") return chp;
		else if (str == "spc") return spc;
		else if (str == "alc") return alc;
		else return err;
	}

	void split_str2vec(std::vector<string> *vec_str, std::string str) {
		std::string token;
		std::istringstream ss(str);
		while (std::getline(ss, token, ' ')) {
			vec_str->push_back(token);
		}
	}


/* Open Action */
	void erase_op_code(std::vector<string> &vec) {
		vec.erase(vec.begin() + 0);
	}

	int extract_table_id(std::vector<string> &vec) {
		int id = stoi(vec.at(0), nullptr, 10);
		vec.erase(vec.begin() + 0);
		return id;
	}

	void create_customers(std::vector<string> argv, std::vector<Customer *> &vec_customers)
	{
		Customer *customer;
		int customer_id = 0;
		std::string customer_name, customer_str_type;

		for (std::vector<string>::const_iterator i = argv.begin(); i != argv.end(); ++i) {
			stringstream ss(*i);

			getline(ss, customer_name, ',');
			getline(ss, customer_str_type, ',');
			CustomerType customer_type = convert_to_customer(customer_str_type);

			cout << "Create NEW customer: name = " << customer_name << " type = " << customer_type << endl;
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
