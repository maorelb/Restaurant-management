
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include<iostream>
#include<fstream>
#include<sstream>


class BaseAction;
class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void create_vector_of_customers(std::vector<std::string> &tokens,int customer_id);

    virtual ~Restaurant(); // destructor
    Restaurant(const Restaurant& other); // copy constructor
    Restaurant& operator=(const Restaurant& other); // copy assigment operator
    Restaurant(Restaurant&& other); //move copy constructor
    Restaurant& operator=(Restaurant && other) ;// move assigment operator

private:
    std::vector<Customer*> customerlist;  //TEMP VECTOR NOT HEADER FILES EMPTY
    int numoftables;
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif