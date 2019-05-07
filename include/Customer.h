
// Created by maor on 10/11/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"
#include <algorithm>
#include <iostream>
typedef std::pair<int, int> DishPair; // A list of pairs for each alc dish in the menu <dish id, dish price>

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer()=0;
    virtual Customer* clone()=0;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual~ VegetarianCustomer();
    virtual VegetarianCustomer* clone();
private:
    int veg_dish_id;
    int most_expensive_bvg_id;
    bool firstOrder;
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual~ CheapCustomer();
    virtual CheapCustomer* clone();
private:
    bool firstOrder;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual ~SpicyCustomer();
    virtual SpicyCustomer* clone();

private:
    int cheapest_bvg_id;
    int most_expensive_spc_id;
    bool firstOrder;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    virtual~ AlchoholicCustomer();
    virtual AlchoholicCustomer* clone ();
private:
    bool firstOrder;
    std::vector<DishPair> alcList;
};
#endif