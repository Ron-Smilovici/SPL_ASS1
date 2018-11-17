#include "Restaurant.h"

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

//default constructor
Restaurant::Restaurant() {}
//parameterized constructor
Restaurant::Restaurant(const std::string &configFilePath) : number_of_tables(0), customer_arrived_so_far(0)
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
		//cout << "Finishing parsing configuration file" << endl;
	}
	else {
		std::cerr << "Couldn't open config file " << configFilePath << " for reading.\n";
	}
}

//copy constructor
Restaurant::Restaurant(const Restaurant &restaurant)
{
	open = restaurant.open;
	for (int i = 0; i < static_cast<int>(restaurant.tables.size()); i++) {

	}
}

// Destructor
Restaurant::~Restaurant() 
{
	std::cout << "Destructor Restaurant was called" << endl;
	for (std::vector<Table *>::const_iterator i = tables.begin(); i != tables.end(); ++i)
		delete *i;
	tables.clear();
	menu.clear();

	for (std::vector<BaseAction *>::const_iterator i = actionsLog.begin(); i != actionsLog.end(); ++i)
		delete *i;
	actionsLog.clear();
	std::cout << "Destrcutor Restaurant finished" << endl;
	system("pause");
}

void Restaurant::parsingTables(string tables_capacity) 
{
	string capacity;
	stringstream ss(tables_capacity);

	while (getline(ss, capacity, DELIMITER_COMMA))
	{
		// create a new table with certain capacity
		Table * table = new Table(stoul(capacity));
		
		//std::cout << "create a NEW table with capacity " << stoul(capacity) << ", table is_open? " << table->isOpen() << endl;

		// insert table to the vector of tables
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

	// Create a Dish
	Dish dish(dish_id, name, stoul(price), convert_to_dish(type));

	// Add the Dish to the menu vector
	menu.push_back(dish);
	//cout << "name = " << name << " type= " << type << " price = " << price << endl;
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

	while (!finish && getline(cin, user_input)) {
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
				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is already open, this action should result in an error */
				if (!is_table_id_valid(table_id) || is_table_open(table_id)) {
		
					cout << "Table does not exist or is already open" << endl;
					//cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					continue;
				}

				/* check the cpacity of the table */
				if (argv.size() > static_cast<int>((this->getTable(table_id))->getCapacity()))
				{
					cout << "Table does not exist or is already open" << endl;
					//cout << "requsted table for " << argv.size() << " persons. Table capacity = " << this->getTable(table_id)->getCapacity() << endl;
					continue;
				}

				// fill customer vector
				create_customers(argv, vec_customers, *this);
				ba = new OpenTable(table_id, vec_customers);
				
			break;
			case ORDER:
				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is closed, this action should result in an error */
				if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
					cout << "Table does not exist or is already open" << endl;
					//cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					continue;
				}

				if (!(this->getTable(table_id)))
				{
					cout << "error- table object doesn't exist, shouldn't happen" << endl;
				}

				ba = new Order(table_id);
			break;
			case MOVE:
				int src_id, dst_id, customer_id;
				//int src, int dst, int customerId
				src_id = extract_table_id(argv);
				dst_id = extract_table_id(argv);
				customer_id = extract_table_id(argv);
				//cout << "src=" << src_id << ", dst=" << dst_id << ", customer_id=" << customer_id << endl;
				/* If the src table doesn't exist or is already closed and
					If the dst table doesn't exist or is already closed this action should result in an error 
					IF the customer ID doesn't exist in the src table id
					If the destination table has no available seats for additional customers */

				if (!is_table_id_valid(src_id) || !is_table_open(src_id) ||
					!is_table_id_valid(dst_id) || !is_table_open(dst_id) ||
					!(this->getTable(src_id)->getCustomer(customer_id)) ||
					this->getTable(dst_id)->getCapacity() <= static_cast<int>(this->getTable(dst_id)->getCustomers().size())) {

					cout << "Cannot move customer" << endl;
					/*cout << "table id = " << src_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					if (!(this->getTable(src_id)->getCustomer(customer_id)))
						cout << "NULL" << endl;
					cout << "this->getTable(dst_id)->getCapacity() = " << this->getTable(dst_id)->getCapacity() << endl;
					cout << "this->getTable(dst_id)->getCustomers().size() = " << this->getTable(dst_id)->getCustomers().size() << endl;*/
					continue;
				}
				
				ba = new MoveCustomer(src_id, dst_id, customer_id);
			break;
			case CLOSEALL:
				ba = new CloseAll();
				finish = true;
			break;
			case CLOSE:
				table_id = extract_table_id(argv);

				/* If the table doesn't exist or is already closed, this action should result in an error */
				if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
					cout << "Table does not exist or is already open" << endl;
					//cout << "table id = " << table_id << " restaurant number of tables = " << this->getNumOfTables() << endl;
					//cout << "table is_open?" << this->getTable(table_id)->isOpen() << endl;
					continue;
				}

				ba = new Close(table_id);
				break;
			case MENU:
				ba = new PrintMenu();
				break;
			case STATUS:
				table_id = extract_table_id(argv);
				ba = new PrintTableStatus(table_id);
				break;
			case BACKUP:
				ba = new BackupRestaurant();
				break;
			default:
				break;
		}

		ba->act(*this);
		cout << ba->toString();

		// log the action in the log
		actionsLog.push_back(ba);
		//cout << endl;
	}
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

void Restaurant::setOpen(bool value)
{
	this->open = value;
}

// Return a reference to the history of actions
//const std::vector<BaseAction*>& Restaurant::getActionsLog() const{}

std::vector<Dish>& Restaurant::getMenu() 
{
	return this->menu;
}

void Restaurant::set_customer_arrived_so_far(int number_of_customers) 
{
	this->customer_arrived_so_far = number_of_customers;
}

int Restaurant::get_customer_arrived_so_far() { return this->customer_arrived_so_far; }

/* Added methods */
DishType Restaurant::convert_to_dish(const std::string& str)
{
	if (str == "VEG") return VEG;
	else if (str == "SPC") return SPC;
	else if (str == "BVG") return BVG;
	else if (str == "ALC") return ALC;
	else return ERROR_DISH;
}

Actions Restaurant::convert_to_action(const std::string& str)
{
	//OPEN, ORDER, MOVE, CLOSE, CLOSEALL, MENU, STATUS, LOG, BACKUP, RESTORE
	if (str == "open") return OPEN;
	else if (str == "order") return ORDER;
	else if (str == "move") return MOVE;
	else if (str == "close") return CLOSE;
	else if (str == "closeall") return CLOSEALL;
	else if (str == "status") return STATUS;
	else if (str == "backup") return BACKUP;
	else if (str == "menu") return MENU;
	else return ERROR_ACTION;

}

CustomerType convert_to_customer(const std::string& str) 
{
	if (str == "veg") return veg;
	else if (str == "chp") return chp;
	else if (str == "spc") return spc;
	else if (str == "alc") return alc;
	else return err;
}

void Restaurant::split_str2vec (std::vector<string> * vec_str, std::string str)
{
	std::string token;
	std::istringstream ss(str);
	while (std::getline(ss, token, ' ')) {
		vec_str->push_back(token);
	}
}

/* Open Action */
void Restaurant::erase_op_code(std::vector<string> & vec)
{
	vec.erase(vec.begin() + 0); 
}

int Restaurant::extract_table_id(std::vector<string> & vec)
{
	int id = stoi(vec.at(0), nullptr, 10);
	vec.erase(vec.begin() + 0);
	return id;
}

void Restaurant::create_customers(std::vector<string> argv, std::vector<Customer *> & vec_customers, Restaurant & res)
{
	Customer * customer;
	std::string customer_name, customer_str_type;

	for (std::vector<string>::const_iterator i = argv.begin(); i != argv.end(); ++i) {
		stringstream ss(*i);

		getline(ss, customer_name, ',');
		getline(ss, customer_str_type, ',');
		CustomerType customer_type = convert_to_customer(customer_str_type);

		//cout << "Create NEW customer: name = " << customer_name << " type = " << customer_type << " id " << res.get_customer_arrived_so_far() << endl;
		switch (customer_type)
		{
		case veg:
			customer = new VegetarianCustomer(customer_name, res.get_customer_arrived_so_far());
			break;
		case chp:
			customer = new CheapCustomer(customer_name, res.get_customer_arrived_so_far());
			break;
		case spc:
			customer = new SpicyCustomer(customer_name, res.get_customer_arrived_so_far());
			break;
		case alc:
			customer = new AlchoholicCustomer(customer_name, res.get_customer_arrived_so_far());

			break;
		}
		vec_customers.push_back(customer);
		res.set_customer_arrived_so_far(res.get_customer_arrived_so_far() + 1);
	}
}


/* Other Actions */