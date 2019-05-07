//
// Created by maor on 10/11/18.
//

#include "../include/Dish.h"
using namespace std;
Dish::Dish(): id(0),name(""),price(0),type(VEG){}
int Dish::getId() const { return id;}
std::string Dish::getName() const { return name;}
int Dish::getPrice() const { return price;}
DishType Dish::getType() const { return type;}


Dish::Dish(int d_id, string d_name, int d_price, DishType d_type):id(d_id),name(d_name),price(d_price),type(d_type){}

DishType Dish::convert(const std::string &str)// convert string to enum ,default VEG
{
    if(str=="ALC")
        return ALC;
    if(str=="SPC")
        return SPC;
    if(str=="BVG")
        return BVG;
    if(str=="VEG")
        return VEG;
    return VEG;
}


