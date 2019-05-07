//
// Created by maor on 10/11/18.
//

#include "../include/Action.h"
extern Restaurant* backup;

using namespace std;
void BaseAction::complete() { status=COMPLETED; }
void BaseAction::error(std::string errorMsg1) {
    this->errorMsg=errorMsg1;
    cout<<errorMsg1<<endl;status=ERROR; } // gets sets builders
ActionStatus BaseAction::getStatus() const { return status; }
BaseAction::BaseAction():errorMsg(""),status(PENDING){}
std::string BaseAction::getErrorMsg() const { return errorMsg; }
BaseAction::~BaseAction()=default;
std::string BaseAction::getMsg() {
    return errorMsg;
}

//********************************OPEN TABLE**********************************
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList) : tableId(id),customers(customersList){}

void OpenTable::act(Restaurant &restaurant) {
    if(tableId < 0 || tableId >= restaurant.getNumOfTables() ||
       unsigned (restaurant.getTable(tableId)->getCapacity())<customers.size()||restaurant.getTable(tableId)->isOpen()){

        error("Error: Table does not exist or is already open");}
    else{
        restaurant.getTable(tableId)->openTable();
        for(unsigned int i=0;i<customers.size();i++){
            restaurant.getTable(tableId)->addCustomer(customers.at(i)->clone());}

        complete();
    }

}
std::string OpenTable::toString() const {
    string str="";
    for(unsigned int i=0;i<customers.size();i++){
        str=str +customers.at(i)->getName()+","+customers.at(i)->toString()+" ";}
    return "open "+ to_string(tableId) +" "+ str ; }


OpenTable::~OpenTable() {
    for(Customer * c: customers){
        if(c!= nullptr)
            delete c;}
    customers.clear();

}


OpenTable::OpenTable(OpenTable &other) :tableId(other.tableId),customers(){
    for(unsigned int i=0;i<other.customers.size();i++){
        customers.push_back(other.customers.at(i)->clone());}

}


//********************************************Move*********************************

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTable(src),dstTable(dst),id(customerId) {}

void MoveCustomer::act(Restaurant &restaurant) {
    if( (restaurant.getNumOfTables()>=srcTable) & (restaurant.getNumOfTables()>=dstTable)) // TABLE EXISTS CHECK*********
        if(((restaurant.getTable(srcTable)->isOpen())&(restaurant.getTable(dstTable)->isOpen()))&
           ((restaurant.getTable(dstTable)->getCustomers().size())<(unsigned int)(restaurant.getTable(dstTable)->getCapacity())))
        {
            bool flag = false;// Both tables are open and destination table has space CHECK********
            for (unsigned int i = 0; i < restaurant.getTable(srcTable)->getCustomers().size(); i++){
                if (restaurant.getTable(srcTable)->getCustomers().at(i)->getId() == id)
                    flag = true;}// id customer exists in source table  CHECK*********

            if (flag)
            {
                vector<OrderPair> pass;
                for(unsigned int i=0;i<restaurant.getTable(srcTable)->getOrders().size();i++){
                    if(restaurant.getTable(srcTable)->getOrders().at(i).first==id)
                        pass.push_back(make_pair(id,restaurant.getTable(srcTable)->getOrders().at(i).second));}
                restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));
                restaurant.getTable(dstTable)->InsertOrders(pass);
                restaurant.getTable(srcTable)->removeCustomer(id);
                if(restaurant.getTable(srcTable)->getCustomers().size()==0)
                    restaurant.getTable(srcTable)->closeTable();
                complete();
            }

            else error("Error: Cannot move customer");
        }
        else error("Error: Cannot move customer");
    else error("Error: Cannot move customer");
}

std::string MoveCustomer::toString() const {
    return "move "+to_string(srcTable) +" "+to_string(dstTable)+" "+to_string(id)+" ";
}

//*****************************************************ORDER******************************************
Order::Order(int id) : tableId(id){}

void Order::act(Restaurant &restaurant) {
    if(tableId < 0 || tableId >= restaurant.getNumOfTables() || !restaurant.getTable(tableId)->isOpen())
        error("Error: Table does not exist or is not open");
    else{
        restaurant.getTable(tableId)->order(restaurant.getMenu());
        complete();
    }
}

std::string Order::toString() const {
    return "order "+to_string(tableId)+" ";
}
//****************************************printmenu********************************
PrintMenu::PrintMenu() {}

void PrintMenu::act(Restaurant &restaurant) {
    string str;
    for(unsigned int i=0;i<restaurant.getMenu().size();i++) {
        DishType type=restaurant.getMenu().at(i).getType();
        switch(type){
            case (0):
                str="VEG";
                break;
            case(1):
                str="SPC";
                break;
            case(2):
                str="BVG";
                break;
            case(3):
                str="ALC";
                break;
        }
        cout << restaurant.getMenu().at(i).getName() << " " << str << " " <<
             restaurant.getMenu().at(i).getPrice() <<"NIS"<<endl;
    }
    complete();
}

std::string PrintMenu::toString() const {
    return "menu ";
}
//************************************ print table status*************************
PrintTableStatus::PrintTableStatus(int id):tableId(id) {}


void PrintTableStatus::act(Restaurant &restaurant) {
    cout<<"Table "<<tableId<<" status: "<<restaurant.getTable(tableId)->getStatus()<<endl;
    if(restaurant.getTable(tableId)->getStatus()=="open") {
        cout << "Customers:" << endl;
        for (unsigned int i = 0; i < restaurant.getTable(tableId)->getCustomers().size(); i++) {
            cout << restaurant.getTable(tableId)->getCustomers().at(i)->getId() << " " <<
                 restaurant.getTable(tableId)->getCustomers().at(i)->getName() << endl;
        }
        cout << "Orders:" << endl;
        vector<OrderPair> &orders = restaurant.getTable(tableId)->getOrders();
        int Bill = 0;
        for (unsigned int i = 0; i < orders.size(); i++) {
            cout << orders.at(i).second.getName() << " ";
            cout << orders.at(i).second.getPrice() << "NIS ";
            cout << orders.at(i).first << endl;
            Bill += orders.at(i).second.getPrice();
        }
        cout << "Current Bill: " << Bill << "NIS" << endl;
    }
    complete();
}
std::string PrintTableStatus::toString() const {
    return "status "+to_string(tableId)+" ";
}
//*****************************************************close***************************************
Close::Close(int id):tableId(id) {}

void Close::act(Restaurant &restaurant) {
    if(tableId < 0 || tableId >= restaurant.getNumOfTables() || !restaurant.getTable(tableId)->isOpen())
        error("Error: Table does not exist or is not open");
    else{
        cout<<"Table "<<tableId<<" was closed. Bill "<<restaurant.getTable(tableId)->getBill()<<"NIS"<<endl;
        restaurant.getTable(tableId)->closeTable();
        complete();
    }
}

std::string Close::toString() const {
    return "close "+ to_string(tableId)+" ";
}

//**********************************close all**********************
CloseAll::CloseAll() {}

void CloseAll::act(Restaurant &restaurant) {
    for(unsigned int i=0;i<(unsigned int)(restaurant.getNumOfTables());i++){
        if (restaurant.getTable(i)->isOpen()) {
            BaseAction *action = new Close(i);
            action->act(restaurant);
            delete action;
        }}
    complete();
}

std::string CloseAll::toString() const {
    return std::__cxx11::string();
}

//************************************LOG***********************************
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant) {
    string str = "";
    for(unsigned int i=0;i<restaurant.getActionsLog().size();i++)
    {
        str =restaurant.getActionsLog().at(i)->toString();
        if(restaurant.getActionsLog().at(i)->getStatus()==ERROR)
            str=str + restaurant.getActionsLog().at(i)->getMsg();
        else
            str=str +"Completed";
        cout<<str<<endl;
        str="";
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return  "log ";
}

//***********************************BACKUP********************************

BackupRestaurant::BackupRestaurant() {}
void BackupRestaurant::act(Restaurant &restaurant) {
    if(backup!= nullptr)
        delete backup;
    backup=new Restaurant(restaurant);
    complete();
}

std::string BackupRestaurant::toString() const {
    return "backup ";
}

//**********************************RESTORE RESTAURANT*****************************
RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup==nullptr)
        error("Error: No backup available");
    else {
        restaurant= *backup;
        complete();
    }

}
std::string RestoreResturant::toString() const {
    return "restore ";
}

//****************************RULE OF FIVE****************


Order::~Order() {

}

MoveCustomer::~MoveCustomer() {

}

Close::~Close() {

}

CloseAll::~CloseAll() {

}

PrintMenu::~PrintMenu() {

}

PrintTableStatus::~PrintTableStatus() {

}

PrintActionsLog::~PrintActionsLog() {

}

BackupRestaurant::~BackupRestaurant() {

}

RestoreResturant::~RestoreResturant() {

}

BaseAction *BaseAction::clone() {return 0;}


OpenTable *OpenTable::clone() {
    return new OpenTable(*this);
}


Order *Order::clone() {
    return new Order(*this);
}

MoveCustomer *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

Close *Close::clone() {
    return new Close(*this);
}

CloseAll *CloseAll::clone() {
    return new CloseAll(*this);
}

PrintMenu *PrintMenu::clone() {
    return new PrintMenu(*this);

}

PrintTableStatus *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

PrintActionsLog *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

BackupRestaurant *BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}

RestoreResturant *RestoreResturant::clone() {
    return new RestoreResturant(*this);
}
