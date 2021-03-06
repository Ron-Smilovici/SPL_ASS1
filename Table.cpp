#include "Table.h"

Table::Table(int t_capacity) : capacity(t_capacity) , open(false) {}
//copy constructor
Table::Table(const Table &table)
{
    open = table.open;
    capacity = table.capacity;

    for (int i = 0; i < static_cast<int>(table.orderList.size()); ++i) {
        orderList.push_back(table.orderList.at(i));
    }

    for (int i = 0; i < static_cast<int>(table.customersList.size()); ++i) {
        Customer* temp = table.customersList.at(i);
        Customer* cloned = temp->clone();
        customersList.push_back(cloned);
    }
}
//copy assignment operator
Table& Table::operator=(const Table &table) {
    open = table.open;
    capacity = table.capacity;

    for (int i = 0; i < static_cast<int>(table.orderList.size()); ++i) {
        orderList.push_back(table.orderList.at(i));
    }

    for (int i = 0; i < static_cast<int>(table.customersList.size()); ++i) {
        Customer* temp = table.customersList.at(i);
        Customer* cloned = temp->clone();
        customersList.push_back(cloned);
    }

    return *this;
}

//move constructor
Table::Table(Table &&table)
{
    open = table.open;
    capacity = table.capacity;

    for (int i = 0; i < static_cast<int>(table.orderList.size()); ++i) {
        orderList.push_back(table.orderList.at(i));
    }

    for (int i = 0; i < static_cast<int>(table.customersList.size()); ++i) {
        customersList.push_back(table.customersList.at(i));
        table.customersList.at(i) = nullptr;
    }
}

//move assignment operator
Table& Table::operator=(Table &&table) {
    open = table.open;
    capacity = table.capacity;

    for (int i = 0; i < static_cast<int>(table.orderList.size()); ++i) {
        orderList.push_back(table.orderList.at(i));
    }

    for (int i = 0; i < static_cast<int>(table.customersList.size()); ++i) {
        customersList.push_back(table.customersList.at(i));
        table.customersList.at(i) = nullptr;
    }

    return *this;
}

// Destructor
Table::~Table()
{
    for (std::vector<Customer *>::const_iterator i = customersList.begin(); i != customersList.end(); ++i)
        delete *i;
    customersList.clear();
    orderList.clear();
}

int Table::getCapacity() const
{
    return capacity;
}

void Table::addCustomer(Customer* customer)
{
    if (!customer)
        return;
    // add the customer to the customerList
    Customer * customerTemp = customer->clone();
    this->customersList.push_back(customerTemp);
}

void Table::removeCustomer(int id)
{
    std::vector<OrderPair> tmp;
    int customer_index_inside_customersList = -1;
    // search for customer id in the customersList to remove
    for (std::vector<Customer *>::const_iterator i = this->customersList.begin();
         i != this->customersList.end();
         ++i)
    {
        customer_index_inside_customersList++;
        if (id == (*i)->getId()) {
            break;
        }
    }

    this->customersList.erase(customersList.begin() + customer_index_inside_customersList);

    // needs to remove the dishes that were orderd by customer id
    for (std::vector<OrderPair>::const_iterator i = orderList.begin(); i != orderList.end(); ++i)
    {
        if ((*i).first == id)
        {
            continue;
        }
        tmp.push_back(*i);
    }

    this->orderList.clear();
    this->orderList = tmp;
}

Customer* Table::getCustomer(int id)
{
    Customer * customer_to_find = NULL;

    if (id < 0)
        return NULL;

    // search for customer id in the customersList
    for (std::vector<Customer *>::const_iterator i = this->customersList.begin();
         i != this->customersList.end();
         ++i)
    {
        if (id == (*i)->getId()) {
            customer_to_find = *i;
            break;
        }
    }
    return customer_to_find;
}

std::vector<Customer*>& Table::getCustomers()
{
    return this->customersList;
}

std::vector<OrderPair>& Table::getOrders()
{
    return this->orderList;
}

void Table::order(const std::vector<Dish> &menu)
{
    std::vector<Customer*> vec_customers;

    vec_customers = this->getCustomers();
    for (std::vector<Customer*>::const_iterator i = vec_customers.begin(); i != vec_customers.end(); ++i)
    {
        // Execute order for each customer i in the table and return a vector
        // of dishes id that the customer ordered
        std::vector<int> ordered_dishes_id = (*i)->order(menu);

        // Create pairs for all the dishes customer i ordered
        for (std::vector<int>::const_iterator j = ordered_dishes_id.begin(); j != ordered_dishes_id.end(); ++j)
        {
            pair <int, Dish> ordered_dish((*i)->getId(), menu.at(*j));/// here needs to fix the customer id
            // push the pair to vector orderList
            (this->getOrders()).push_back(ordered_dish);
            cout << (*i)->getName() << " ordered " << menu.at(*j).getName() << endl;
        }
    }
}

void Table::openTable()
{
    open = true;
}

void Table::closeTable()
{
    open = false;
}

int Table::getBill()
{
    int sum = 0;
    std::vector<OrderPair> op = this->getOrders();

    for (std::vector<OrderPair>::const_iterator i = op.begin(); i != op.end(); ++i)
    {
        sum += (*i).second.getPrice();
    }

    return sum;
}

bool Table::isOpen()
{
    return open;
}

std::string Table::toString() const
{
    return "capacity = " + to_string(capacity);
}

std::vector<OrderPair> Table::getCustomerOrders(int customer_id)
{
    std::vector<OrderPair> tmp;
    for (std::vector<OrderPair>::const_iterator i = this->orderList.begin(); i != this->orderList.end(); ++i)
    {
        if ((*i).first == customer_id)
            tmp.push_back(*i);
    }
    return tmp;
}

void Table::updateOrders(std::vector<OrderPair> orders)
{
    for (std::vector<OrderPair>::const_iterator i = orders.begin(); i != orders.end(); ++i)
        this->getOrders().push_back(*i);
}