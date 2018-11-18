#include "Restaurant.h"

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
        Table* table = new Table(*restaurant.tables.at(i));
        tables.push_back(table);
    }

    for (int i = 0; i < static_cast<int>(restaurant.menu.size()); i++) {
        menu.push_back(restaurant.menu.at(i));
    }

    for (int i =0; i < static_cast<int>(restaurant.actionsLog.size()); i++) {
        BaseAction* temp = restaurant.actionsLog.at(i);
        BaseAction* cloned = temp->clone();
        actionsLog.push_back(cloned);
    }

    number_of_tables = restaurant.number_of_tables;
}

// Destructor
Restaurant::~Restaurant()
{
    for (std::vector<Table *>::const_iterator i = tables.begin(); i != tables.end(); ++i)
        delete *i;
    tables.clear();
    menu.clear();

    for (std::vector<BaseAction *>::const_iterator i = actionsLog.begin(); i != actionsLog.end(); ++i)
        delete *i;
    actionsLog.clear();
}

//copy assignment operator
Restaurant& Restaurant::operator=(const Restaurant & restaurant) {

    // initialization of all members
    for(Table* ptable : tables){
        delete(ptable);
    }
    tables.clear();

    menu.clear();

    for(BaseAction* pba : actionsLog){
        delete(pba);
    }
    actionsLog.clear();

    // setting values
    open = restaurant.open;

    for(Table* ptable : restaurant.tables){
        Table* table = new Table(*ptable);
        tables.push_back(table);
    }

    for(const Dish & dish : restaurant.menu){
        menu.push_back(dish);
    }

    for(BaseAction* pba : restaurant.actionsLog){
        BaseAction* cloned = pba->clone();
        actionsLog.push_back(cloned);
    }

    number_of_tables = restaurant.number_of_tables;

    return *this;
}

//move constructor
Restaurant::Restaurant(Restaurant &&restaurant)
{
    open = restaurant.open;

    for(int i = 0; i < static_cast<int>(restaurant.tables.size()); i++) {
        tables.push_back(restaurant.tables.at(i));
        restaurant.tables.at(i) = nullptr;
    }

    for (int i = 0; i < static_cast<int>(restaurant.menu.size()); i++) {
        menu.push_back(restaurant.menu.at(i));
    }

    for (int i = 0; i < static_cast<int>(restaurant.actionsLog.size()); i++){
        actionsLog.push_back(restaurant.actionsLog.at(i));
        restaurant.actionsLog.at(i) = nullptr;
    }

    number_of_tables = restaurant.number_of_tables;
}

//move assignment operator
Restaurant& Restaurant::operator=(Restaurant &&restaurant) {

    // initialization of all members
    for(Table* ptable : tables){
        delete(ptable);
    }
    tables.clear();

    menu.clear();

    for(BaseAction* pba : actionsLog){
        delete(pba);
    }
    actionsLog.clear();

    // setting values

    open = restaurant.open;

    for(int i = 0; i < static_cast<int>(restaurant.tables.size()); i++) {
        tables.push_back(restaurant.tables.at(i));
        restaurant.tables.at(i) = nullptr;
    }

    for (int i = 0; i < static_cast<int>(restaurant.menu.size()); i++) {
        menu.push_back(restaurant.menu.at(i));
    }

    for (int i = 0; i < static_cast<int>(restaurant.actionsLog.size()); i++){
        actionsLog.push_back(restaurant.actionsLog.at(i));
        restaurant.actionsLog.at(i) = nullptr;
    }

    number_of_tables = restaurant.number_of_tables;

    return *this;
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
                // fill customer vector
                create_customers(argv, vec_customers, *this);
                ba = new OpenTable(table_id, vec_customers);

                /* If the table doesn't exist or is already open, this action should result in an error */
                if (!is_table_id_valid(table_id) || is_table_open(table_id)) {
                    ba->activate_error("Table does not exist or is already open");
                    actionsLog.push_back(ba);
                    continue;
                }

                /* check the cpacity of the table */
                if (static_cast<int>(argv.size()) > (this->getTable(table_id)->getCapacity()))
                {
                    ba->activate_error("Table does not exist or is already open");
                    actionsLog.push_back(ba);
                    continue;
                }

                break;

            case ORDER:
                table_id = extract_table_id(argv);
                ba = new Order(table_id);
                /* If the table doesn't exist or is closed, this action should result in an error */
                if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
                    ba->activate_error("Table does not exist or is already open");
                    actionsLog.push_back(ba);
                    continue;
                }

                if (!(this->getTable(table_id)))
                {
                    ba->activate_error("Table does not exist or is already open");
                    actionsLog.push_back(ba);
                    continue;
                }

                break;

            case MOVE:
                int src_id, dst_id, customer_id;
                src_id = extract_table_id(argv);
                dst_id = extract_table_id(argv);
                customer_id = extract_table_id(argv);

                ba = new MoveCustomer(src_id, dst_id, customer_id);
                if (!is_table_id_valid(src_id) || !is_table_open(src_id) ||
                    !is_table_id_valid(dst_id) || !is_table_open(dst_id) ||
                    !(this->getTable(src_id)->getCustomer(customer_id)) ||
                    this->getTable(dst_id)->getCapacity() <= static_cast<int>(this->getTable(dst_id)->getCustomers().size())) {

                    ba->activate_error("Cannot move customer");
                    actionsLog.push_back(ba);
                    continue;
                }

                break;

            case CLOSEALL:
                ba = new CloseAll();
                ba->act(*this);
                finish = true;
                break;

            case CLOSE:
                table_id = extract_table_id(argv);
                ba = new Close(table_id);
                /* If the table doesn't exist or is already closed, this action should result in an error */
                if (!is_table_id_valid(table_id) || !is_table_open(table_id)) {
                    ba->activate_error("Table does not exist or is already open");
                    actionsLog.push_back(ba);
                    continue;
                }

                break;

            case MENU:
                ba = new PrintMenu();
                break;

            case STATUS:
                table_id = extract_table_id(argv);
                ba = new PrintTableStatus(table_id);
                break;

            case LOG:
                ba = new PrintActionsLog();
                break;

            case BACKUP:
                ba = new BackupRestaurant();
                break;

            case RESTORE:
                ba = new RestoreResturant();
                break;

            default:
                break;
        }

        ba->act(*this);


        // log the action in the log
        actionsLog.push_back(ba);
    }
}

int Restaurant::getNumOfTables() const
{
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
const std::vector<BaseAction*>& Restaurant::getActionsLog() const
{
    return actionsLog;
}

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
    else if (str == "restore") return RESTORE;
    else if (str == "log") return LOG;

    else return ERROR_ACTION;

}

CustomerType Restaurant::convert_to_customer(const std::string& str)
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

        //cout << "Create NEW customer: name = " << customer_name << " type = " << customer_type << endl;
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
