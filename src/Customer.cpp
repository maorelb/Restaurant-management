
#include "../include/Customer.h"
using namespace std;

//**************************Customer*****************************************
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}

std::string Customer::getName() const { return name;}

int Customer::getId() const {return id;}

Customer::~Customer(){}


//****************************VegeterianCustomer*********************************************

VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id),veg_dish_id(-1)
        ,most_expensive_bvg_id(-1),firstOrder(false){}

VegetarianCustomer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);}

VegetarianCustomer::~VegetarianCustomer() {}

std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    vector<int> order;
    if(!firstOrder) {
        firstOrder=true;
        bool foundVeg = false;
        int bvg_price = INT8_MIN;
        for (unsigned int i = 0; i < menu.size(); i++) {
            if (menu.at(i).getType() == VEG && !foundVeg) {
                veg_dish_id=i;
                foundVeg = true;
            }
            if (menu.at(i).getType() == BVG && menu.at(i).getPrice() > bvg_price) {
                bvg_price = menu.at(i).getPrice();
                most_expensive_bvg_id=i;
            }
        }
    }
    //customer can complete order
    if(veg_dish_id!=-1 && most_expensive_bvg_id!=-1) {
        order.push_back(veg_dish_id);
        order.push_back(most_expensive_bvg_id);
    }
    return order;
}
std::string VegetarianCustomer::toString() const {return "veg";}

//****************************CheapCustomer*****************************************


CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id),firstOrder(false){}
CheapCustomer *CheapCustomer::clone() {
    return new CheapCustomer(*this); }

CheapCustomer::~CheapCustomer(){}


std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    vector<int> order;
    if(!firstOrder) {
        firstOrder=true;
        int price = INT8_MAX;
        int id;
        for (unsigned int j = 0; j < menu.size(); j++) {
            if (menu.at(j).getPrice() < price) {
                price = menu.at(j).getPrice();
                id = j;
            }
        }
        order.push_back(id);
    }
    return order;
}
std::string CheapCustomer::toString() const {return "chp";}

//****************************SpicyCustomer****************************************

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id),cheapest_bvg_id(-1),most_expensive_spc_id(-1)
        ,firstOrder(false){}

SpicyCustomer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);}

SpicyCustomer::~SpicyCustomer() {}

std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    vector<int> order;
    if (!firstOrder) {
        firstOrder=true;
        int spc_price = INT8_MIN, bvg_price = INT8_MAX;
        for (unsigned int i = 0; i < menu.size(); i++){
            if (menu.at(i).getType() == SPC && spc_price < menu.at(i).getPrice()) {
                spc_price = menu.at(i).getPrice();
                most_expensive_spc_id = i;
            }}
        if(most_expensive_spc_id!=-1)
            for(unsigned int i=0;i<menu.size();i++){
                if (menu.at(i).getType() == BVG && menu.at(i).getPrice() < bvg_price) {
                    bvg_price = menu.at(i).getPrice();
                    cheapest_bvg_id = i;
                }}
        //can complete first order
        if (most_expensive_spc_id != -1)
            order.push_back(most_expensive_spc_id);
    }
    else if((firstOrder) && (cheapest_bvg_id!=-1)) {
        // forwards orders
        order.push_back(cheapest_bvg_id);
    }
    return order;
}
std::string SpicyCustomer::toString() const { return  "spc";}

//******************************AlcoholicCustomer***********************************

AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id),firstOrder(false),alcList(){}
std::string AlchoholicCustomer::toString() const { return "alc";}

AlchoholicCustomer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);}

std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    vector<int> order;
    if (!firstOrder) {
        firstOrder = true;
        for (unsigned int i = 0; i < menu.size(); i++) {//init alcList
            if (menu.at(i).getType() == ALC)
                alcList.push_back(make_pair(menu.at(i).getId(),menu.at(i).getPrice()));}
        std::stable_sort(alcList.begin(), alcList.end(), [](DishPair d1, DishPair d2) { //stable_sort alcList
            return d1.second < d2.second; });
    }
    if (alcList.size() > 0) {
        order.push_back(alcList.at(0).first);
        alcList.erase(alcList.begin());
    }
    return order;
}

AlchoholicCustomer::~AlchoholicCustomer() {
    alcList.clear();
}


//**********************************************************************



