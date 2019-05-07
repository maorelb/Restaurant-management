//
// Created by maor on 10/11/18.
//
using namespace std;
#include "../include/Restaurant.h"
extern Restaurant* backup;

int Restaurant::getNumOfTables() const {return numoftables;}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const {return actionsLog;}
Table* Restaurant::getTable(int ind) {return tables.at(ind);}
std::vector<Dish>& Restaurant::getMenu() {return menu;}


//constructor
Restaurant::Restaurant(const std::string &configFilePath):customerlist(),numoftables(0),open(false),
                                                          tables(),menu(),actionsLog(){
    string line,var;
    int flag=0,index=0;
    ifstream myfile(configFilePath);
    string d_name, d_price;
    DishType d_type;
    while(getline(myfile,line)){

        if(line!=(""))
            if(line.at(0)!='#'){
                flag++;
                if(flag==1)
                    numoftables=stoi(line);
                if(flag==2) //stream tables
                {
                    stringstream strt(line);
                    while(getline(strt,var,',')){
                        Table *t=new Table(stoi(var));
                        tables.push_back(t);

                    }
                }
                if(flag>2){ //stream menu
                    stringstream strm(line);
                    getline(strm, var, ',');
                    d_name = var;
                    getline(strm, var, ',');
                    d_type = Dish::convert(var);
                    getline(strm, var, ',');
                    d_price = var;
                    Dish d(index, d_name, stoi(d_price), d_type);
                    menu.push_back(d);
                    index++;
                }
            }    }
    myfile.close();
}

//**********************************************START********************************
void Restaurant::start() {

    BaseAction* action;
    int customers_id = 0;
    cout << "Restaurant is now open!" << endl;
    open = true;
    string line;
    while (getline(cin, line)) {
        //seperate action by space
        vector <string> tokens;
        istringstream iss(line);
        do {
            string subs;
            iss >> subs;
            tokens.push_back(subs);
        } while (iss);

        if (tokens.at(0) == "closeall"){
            action=new CloseAll();
            action->act(*this);
            open=false;
            delete action;
            break;
        }
        else if (tokens.at(0) == "open") {
            create_vector_of_customers(tokens, customers_id);//update temp vector
            action = new OpenTable(stoi(tokens.at(1)), customerlist);
            customers_id += tokens.size() - 3;
            customerlist.clear();
        }
        else if (tokens.at(0) == "order") {
            action=new Order(stoi(tokens.at(1)));
        }

        else if (tokens.at(0) == "move") {
            action = new MoveCustomer(stoi(tokens.at(1)),stoi(tokens.at(2)),stoi(tokens.at(3)) );
        }
        else if(tokens.at(0)=="menu"){
            action=new PrintMenu();

        }
        else if (tokens.at(0) == "status") {
            action=new PrintTableStatus(stoi(tokens.at(1)));
        }

        else if(tokens.at(0)=="close"){
            action=new Close(stoi(tokens.at(1)));
        }
        else if (tokens.at(0) == "log") {
            action=new PrintActionsLog();

        }
        else if (tokens.at(0) == "backup") {
            action=new BackupRestaurant();
        }
        else if (tokens.at(0)=="restore"){
            action=new RestoreResturant();
        }

        action->act(*this);
        actionsLog.push_back(action);
    }
}

void Restaurant::create_vector_of_customers(std::vector<std::string> &tokens, int customer_id) {
    string name, type;

    int size = tokens.size(), i = 2;
    while (i < size - 1) {
        //seperate by comma
        stringstream pair(tokens.at(i));
        getline(pair, name, ',');
        getline(pair, type, ',');
        if(type=="veg")
            customerlist.push_back( new VegetarianCustomer (name,customer_id));
        if(type=="chp")
            customerlist.push_back ( new CheapCustomer (name,customer_id));
        if(type=="alc")
            customerlist.push_back ( new AlchoholicCustomer(name,customer_id));
        if(type=="spc")
            customerlist.push_back ( new SpicyCustomer(name,customer_id));

        i++;
        customer_id++;
    }

}

// *************************destructor*****************************
Restaurant::~Restaurant() {

    for(unsigned int i=0 ; i<customerlist.size();i++){
        delete customerlist.at(i);
        customerlist.clear();}


    for(unsigned int i=0 ; i<tables.size();i++){
        delete tables.at(i);
    }
    tables.clear();

    for(unsigned int i=0 ; i<actionsLog.size();i++){
        if (actionsLog.at(i) != nullptr) {
            delete actionsLog.at(i);
            actionsLog.at(i) = nullptr;
        }}
    actionsLog.clear();

}
//***************copy constructor************************
Restaurant::Restaurant(const Restaurant &other):customerlist(),numoftables(other.numoftables)
        ,open(other.open),tables(),menu(),actionsLog(){

    for(unsigned int i=0;i<other.customerlist.size();i++){
        customerlist.push_back(other.customerlist.at(i)->clone());
    }
    for(unsigned int i=0;i<other.tables.size();i++){
        Table * t =new Table(*other.tables.at(i));
        tables.push_back(t);
    }
    for(unsigned int i=0;i<other.menu.size();i++){
        menu.push_back(other.menu.at(i));}

    for(unsigned int i=0;i<other.actionsLog.size();i++) {
        actionsLog.push_back(other.actionsLog.at(i)->clone());
    }
}
//******************COPY ASSIGNMENT OPERATOR************************

Restaurant &Restaurant::operator=(const Restaurant &other)  {
    if (this == &other)
        return *this;

    numoftables = other.numoftables;
    open = other.open;
    menu.clear();
    for (unsigned int i = 0; i < other.menu.size(); i++){
        menu.push_back(other.menu.at(i));}

    for (unsigned int i = 0; i < tables.size(); i++){
        if (tables.at(i) != nullptr) {
            delete tables.at(i);
            tables.at(i) = nullptr;
        }}
    tables.clear();

    for (unsigned int i = 0; i < other.tables.size(); i++) {
        tables.push_back(new Table(*other.tables.at(i)));

    }

    for (unsigned int i = 0; i < actionsLog.size(); i++){
        if (actionsLog.at(i) != nullptr) {
            delete actionsLog.at(i);
            actionsLog.at(i) = nullptr;
        }}
    actionsLog.clear();

    for (BaseAction * action:other.actionsLog){
        actionsLog.push_back(action->clone());}
    return *this;

}
//*********************MOVE COPY CONSTRUCTOR****************************

Restaurant::Restaurant(Restaurant &&other):customerlist(),numoftables(other.numoftables)
        ,open(other.open),tables(),menu(),actionsLog(){

    tables=move(other.tables);
    menu=move(other.menu);
    actionsLog = move(other.actionsLog);
}
//***************************MOVE ASSIGMENT OPERATOR***************************
Restaurant &Restaurant::operator=(Restaurant &&other) {
    if(this!=&other){
        menu.clear();
        for (unsigned int i = 0; i < tables.size(); i++){
            if (tables.at(i) != nullptr) {
                delete tables.at(i);
                tables.at(i) = nullptr;
            }}
        tables.clear();

        for (unsigned int i = 0; i < actionsLog.size(); i++){
            if (actionsLog.at(i) != nullptr) {
                delete actionsLog.at(i);
                actionsLog.at(i) = nullptr;
            }}
        actionsLog.clear();
        numoftables=other.numoftables;
        open=other.open;
        tables=move(other.tables);
        menu=move(other.menu);
        actionsLog = move(other.actionsLog);
    }
        return *this;
}


