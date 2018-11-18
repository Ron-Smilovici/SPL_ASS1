#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

// rule of 5
/*
	copy contructor
	move constructor
	Move assignemnt operator
	Copy assignement opertaor
*/


enum ActionStatus {
	PENDING, COMPLETED, ERROR
};

enum Actions {
	OPEN, ORDER, MOVE, CLOSE, CLOSEALL, MENU, STATUS, LOG, BACKUP, RESTORE, ERROR_ACTION
};

//Forward declaration
class Restaurant;

class BaseAction {
public:
	BaseAction();
	BaseAction(const BaseAction &action);//copy constructor
//    BaseAction(BaseAction &&action);//move constructor
//    BaseAction& operator=(const BaseAction & action); //copy assignment operator
//    BaseAction& operator=(BaseAction && action);//move assignment operator
	virtual ~BaseAction();
	virtual BaseAction* clone() = 0; // create a dynamic copy of the specific child action
	ActionStatus getStatus() const;
	virtual void act(Restaurant& restaurant) = 0;
	virtual std::string toString() const = 0;
	ostream& operator<<(ostream& out);
	std::string convert_to_dishtype(int num);
	std::string status_to_string() const ;
	void activate_error(std::string errorMsg);
protected:
	void complete();
	void error(std::string errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};


class OpenTable : public BaseAction {
public:
	~OpenTable();
	OpenTable(int id, std::vector<Customer *> &customersList);
	OpenTable(const OpenTable &openTable);//copy constructor (alon)
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
	Order(int id);
	Order(const Order &order);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	const int tableId;
};


class MoveCustomer : public BaseAction {
public:
	MoveCustomer(int src, int dst, int customerId);
	MoveCustomer(const MoveCustomer &moveCustomer);
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	const int srcTable;
	const int dstTable;
	const int id;
};


class Close : public BaseAction {
public:
	Close(int id);
	Close(const Close &close);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	const int tableId;
	int bill;
};


class CloseAll : public BaseAction {
public:
	CloseAll();
	CloseAll(const CloseAll &closeAll);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	std::stringstream table_bills;
};


class PrintMenu : public BaseAction {
public:
	PrintMenu();
	PrintMenu(const PrintMenu &printMenu);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
};


class PrintTableStatus : public BaseAction {
public:
	PrintTableStatus(int id);
	PrintTableStatus(const PrintTableStatus &printTableStatus);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
	const int tableId;
	std::string open;
	int bill;
	std::string customers;
	std::string dishes;
};


class PrintActionsLog : public BaseAction {
public:
	PrintActionsLog();
	PrintActionsLog(const PrintActionsLog &printActionsLog);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
};


class BackupRestaurant : public BaseAction {
public:
	BackupRestaurant();
	BackupRestaurant(const BackupRestaurant &backupRestaurant);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
private:
};


class RestoreResturant : public BaseAction {
public:
	RestoreResturant();
	RestoreResturant(const RestoreResturant &restoreResturant);//copy constructor
	void act(Restaurant &restaurant);
	std::string toString() const;
	BaseAction* clone(); // create a dynamic copy of the specific child action
};


#endif