//
// Created by maor on 10/11/18.
//

#include "../include/Table.h"
using namespace std;

Table::Table(int t_capacity):capacity(t_capacity),open(false),customersList(),orderList() {}
int Table::getCapacity() const {return capacity;}
bool Table::isOpen() {return open;}
void Table::addCustomer(Customer *customer) { customersList.push_back(customer); } //get and sets build

std::vector<Customer *> &Table::getCustomers() { return customersList; }
void Table::openTable() { open=true; }


void Table::InsertOrders(std::vector<OrderPair> v) {
    for(unsigned int i=0;i<v.size();i++){
        orderList.push_back(make_pair(v.at(i).first,v.at(i).second));}
}

void Table::removeCustomer(int id) {
    for(unsigned int i=0;i<customersList.size();i++){
        if(customersList.at(i)->getId()==id)
            customersList.erase(customersList.begin()+i);}//customer not in table

    vector<OrderPair> temp;
    for(unsigned int i=0;i<orderList.size();i++){
        if(orderList.at(i).first!=id){

            temp.push_back(orderList.at(i));
        }}
    orderList.clear();
    for(unsigned int i=0;i<temp.size();i++){
        orderList.push_back(temp.at(i));
    }
    temp.clear();
}

std::vector<OrderPair> &Table::getOrders() {
    return orderList;
}

Customer *Table::getCustomer(int id) {

    for(unsigned int i=0;i<customersList.size();i++){
        if (customersList.at(i)->getId()==id)
            return customersList.at(i);}
    return nullptr;
}

std::string Table::getStatus() {
    if(isOpen())
        return "open";
    else
        return "closed";
}
int Table::getBill() {
    int Bill=0;
    for(unsigned int i=0;i<orderList.size();i++){
        Bill+=orderList.at(i).second.getPrice();}
    return Bill;
}

//*************closeTable******************************
void Table::closeTable() {
    open =false;
    for(unsigned int i=0;i<customersList.size();i++){
        delete customersList.at(i);}
    customersList.clear();
    orderList.clear();
}

//*******************Order************************************
void Table::order(const std::vector<Dish> &menu) {
    int customer_id, dish_id;
    if(menu.size()>0) {
        for (unsigned int i = 0; i < customersList.size(); i++) {
            customer_id = customersList.at(i)->getId();
            vector<int> order = customersList.at(i)->order(menu);
            for (unsigned int j = 0; j < order.size(); j++) {
                dish_id = order.at(j);
                Dish d = menu.at(dish_id);
                OrderPair p = make_pair(customer_id, d);
                orderList.push_back(p);
                cout << customersList.at(i)->getName() << " ordered " << d.getName() << endl;
            }
        }
    }
}

//***************************DESTRUCTOR****************
Table::~Table() {
    for (unsigned int i = 0; i < customersList.size(); i++){
        if (customersList.at(i) != nullptr)
            delete customersList.at(i);
        customersList.at(i) = nullptr;
    }
    customersList.clear();
    orderList.clear();

}
//***************************COPY CONSTRUCTOR*****************
Table::Table(const Table &other):capacity(other.capacity),open(other.open),customersList(),orderList(){
    orderList.clear();
    for(unsigned int i=0;i<customersList.size();i++){
        if(customersList.at(i)!=nullptr)
            delete customersList.at(i);}

    customersList.clear();


    for(unsigned int i=0;i<other.customersList.size();i++){
        customersList.push_back(other.customersList.at(i)->clone());}

    for(unsigned int i=0;i<other.orderList.size();i++){
        orderList.push_back(other.orderList.at(i));}
}

//*********************MOVE ASSIGNMENT OPERATOR******************
Table &Table::operator=(const Table &other) {
    capacity=other.capacity;
    open=other.open;
    orderList.clear();
    for(unsigned int i=0;i<customersList.size();i++){
        if(customersList.at(i)!=nullptr)
            delete customersList.at(i);
        customersList.at(i)=nullptr;}

    for(unsigned int i=0;i<other.customersList.size();i++){
        customersList.push_back(other.customersList.at(i)->clone());}

    for(unsigned int i=0;i<other.orderList.size();i++){
        orderList.push_back(other.orderList.at(i));}

    return *this;
}
//*****************MOVE CONSTRUCTOR**************************
Table::Table(Table &&other):capacity(other.capacity),open(other.open),customersList(),orderList() {
    customersList=move(other.customersList);
    orderList=move(other.orderList);
}
//**************************MOVE OPERATOR****************************
Table &Table::operator=(Table &&other) {
    if(this!=&other){
     capacity=other.capacity;
     open=other.open;
     orderList.clear();
        for(unsigned int i=0;i<customersList.size();i++){
            if(customersList.at(i)!=nullptr)
                delete customersList.at(i);
            customersList.at(i)=nullptr;}
        customersList=move(other.customersList);
        orderList=move(other.orderList);
    }
        return *this ;
}
