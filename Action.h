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
	ActionStatus getStatus() const;
	virtual void act(Restaurant& restaurant) = 0;
	virtual std::string toString() const = 0;
	ostream& operator<<(ostream& out);
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
	OpenTable(int id, std::vector<Customer *> &customersList);
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
	const int tableId;
	std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
	Order(int id);
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
	const int tableId;
};


class MoveCustomer : public BaseAction {
public:
	MoveCustomer(int src, int dst, int customerId);
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
	const int srcTable;
	const int dstTable;
	const int id;
};


class Close : public BaseAction {
public:
	Close(int id);
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
	const int tableId;
	int bill;
};


class CloseAll : public BaseAction {
public:
	CloseAll();
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
};


class PrintMenu : public BaseAction {
public:
	PrintMenu();
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
};


class PrintTableStatus : public BaseAction {
public:
	PrintTableStatus(int id);
	void act(Restaurant &restaurant);
	std::string toString() const;
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
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
};


class BackupRestaurant : public BaseAction {
public:
	BackupRestaurant();
	void act(Restaurant &restaurant);
	std::string toString() const;
private:
};


class RestoreResturant : public BaseAction {
public:
	RestoreResturant();
	void act(Restaurant &restaurant);
	std::string toString() const;

};


#endif