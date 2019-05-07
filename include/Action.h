//
// Created by maor on 10/11/18.
//
#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Table.h"
#include "Restaurant.h"
enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    std::string getMsg();
    virtual ~ BaseAction();
    virtual BaseAction* clone()  ;

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
    OpenTable(OpenTable& other);
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ OpenTable();
    virtual OpenTable* clone();

private:
    const int tableId;
    std::vector<Customer *> customers;
};

class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~Order();
    virtual Order* clone();
private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ MoveCustomer();
    virtual MoveCustomer* clone();
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
    virtual ~Close();
    virtual Close* clone();
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~CloseAll();
    virtual CloseAll* clone();

private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    virtual ~ PrintMenu();
    std::string toString() const;
    virtual PrintMenu* clone();
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ PrintTableStatus();
    virtual PrintTableStatus* clone();
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ PrintActionsLog();
    virtual PrintActionsLog* clone();
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ BackupRestaurant();
    virtual BackupRestaurant* clone();
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    virtual ~ RestoreResturant();
    virtual RestoreResturant* clone();

};

#endif