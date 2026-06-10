#include "Classes.h"
#include <iostream>
using namespace std;

Address::Address() 
: City("") , Street("") , StreetNo(0) , BuildingNo(0) {}

Address::Address(string c, string s, int sn, int bn) 
: City(c) , Street(s) , StreetNo(sn) , BuildingNo(bn) {}

istream &operator >> (istream &input, Address &a) {
    input >> a.City >> a.Street >> a.StreetNo >> a.BuildingNo;
    return input;
}
ostream &operator << (ostream &output, const Address &a) {
    output << a.City << ", " << a.Street << " " << a.StreetNo << ", No. " << a.BuildingNo << "\n";
    return output;
}

string Address::GetCity() const { return City; }
string Address::GetStreet() const { return Street; }
int Address::GetStreetNo() const { return StreetNo; }
int Address::GetBuildingNo() const { return BuildingNo; }

void Address::SetCity(string c) { City = c; }
void Address::SetStreet(string s) { Street = s; }
void Address::SetStreetNo(int sn) { StreetNo = sn; }
void Address::SetBuildNo(int bn) { BuildingNo = bn; }

Item::Item()
: ID(0) , name("") , etc("") , Price(0.0) , type(Other) , status(InActive) {}

Item::Item(int id, string n, string e, double p, Type t, Status s) 
: ID(id) , name(n) , etc(e) , Price(p) , type(t) , status(s) {}

int Item::GetID() const { return ID; }
string Item::Getname() const { return name; }
string Item::Getdesc() const { return etc; }
double Item::GetPrice() const { return Price; }
Type Item::Gettype() const { return type; }
bool Item::Getstatus() const { return status; }

void Item::SetID(int ID) { this->ID = ID; }
void Item::Setname(string name) { this->name = name; }
void Item::SetDesc(string etc) { this->etc = etc; }
void Item::SetPrice(double price) { this->Price = price; }
void Item::Setype(Type type) { this->type = type; }
void Item::Setstatus(Status status) { this->status = status; }

bool Item::IsActive() {
    if (status) return 1;
    return 0;
}
void Item::Activate() { status = Active; }
void Item::DeActivate() { status = InActive; }

void Item::Display() {
    cout << "  --- Item Details ---\n";
    cout << "     " << type << "[" << status << "]\n";
    cout << "     " << name << "(" << ID << ")\n";
    cout << "     Description: " << etc << endl;
    cout << "     Price: " << Price << endl;
    cout << "  --------------------\n";
}

Menu::Menu() : RestaurantID(0) {}

Menu::Menu(int ID) : RestaurantID(ID) {}

int Menu::GetRestaurantId() { return RestaurantID; }

vector <Item*> Menu::GetItems() {
    return MenuItems;
}

int Menu::FindItemIndex(int id) {
    for (int i=0; i<MenuItems.size(); i++) {
        if (MenuItems[i]->GetID() == id) return i;
    }
    return -1;
}
void Menu::SetRestaurantId(int restaurantId) { RestaurantID = restaurantId; }
void Menu::AddItem(Item* item) { 
    MenuItems.push_back(item);   
}
void Menu::RemoveItem(int itemId) {
    MenuItems.erase(MenuItems.begin() + FindItemIndex(itemId));
}
void Menu::UpdateItem(Item* item) {

}

void Menu::DisplayAll() const {
    for (auto item : MenuItems) {
        item->Display();
    }
}

void Menu::displayActive() const {
    for (auto item : MenuItems) {
        if (item->IsActive()) item->Display();
    }
}

Restaurant::Restaurant()
: ID(0) , name("") , address(Address()) , Prep_Time(-1) , Phone_Number("") , etc("") , menu(Menu()) , status(InActive) {}

Restaurant::Restaurant(int i, string n, Address a, int p, string ph, string e, Menu m, Status s)
: ID(i) , name(n) , address(a) , Prep_Time(p) , Phone_Number(ph) , etc(e) , menu(m) , status(s) {}

void Restaurant::SetID(int ID) { this->ID = ID; }
void Restaurant::Setname(string name) { this->name = name; }
void Restaurant::Setaddress(Address address) { this->address = address; }
void Restaurant::SetPrep(int Prep_Time) { this->Prep_Time = Prep_Time; }
void Restaurant::SetPhone(string PhoneNum) { Phone_Number = PhoneNum; }
void Restaurant::Setdesc(string etc) { this->etc = etc; }
void Restaurant::Setmenu(Menu menu) { this->menu = menu; }
void Restaurant::SetStatus(Status status) { this->status = status; }

int Restaurant::GetID() const { return ID; }
string Restaurant::Getname() const { return name; }
Address Restaurant::Getaddress() const { return address; }
int Restaurant::GetPrep() const { return Prep_Time; }
string Restaurant::GetPhone() const { return Phone_Number; }
string Restaurant::Getdesc() const { return etc; }
Menu& Restaurant::GetMenu() { return menu; }
Status Restaurant::GetStatus() const { return status; }



FoodC::FoodC() : Item() , Prep_Time(-1) {}
FoodC::FoodC(int id, string n, string e, double p, Type t, Status s, int pr)
: Item(id, n, e, p, t, s) , Prep_Time(pr) {}
int FoodC::GetPrep() const { return Prep_Time; }
void FoodC::Display() {
    cout << "  --- Item Details ---\n";
    cout << "     " << type << "[" << status << "]\n";
    cout << "     " << name << "(" << ID << ")\n";
    cout << "     Description: " << etc << endl;
    cout << "     Price: " << Price << endl;
    cout << "     PrepTime: " << Prep_Time << endl;
    cout << "  --------------------\n";
}
void FoodC::SetPrep(int Prep) { Prep_Time = Prep; }

DrinkC::DrinkC() : Item() , Volume(-1) {}
DrinkC::DrinkC(int id, string n, string e, double p, Type t, Status s, double v)
: Item(id, n, e, p, t, s) , Volume(v) {}
double DrinkC::GetVolume() const { return Volume; }
void DrinkC::Display() {
    cout << "  --- Item Details ---\n";
    cout << "     " << type << "[" << status << "]\n";
    cout << "     " << name << "(" << ID << ")\n";
    cout << "     Description: " << etc << endl;
    cout << "     Price: " << Price << endl;
    cout << "     Volume: " << Volume << endl;
    cout << "  --------------------\n";
}
void DrinkC::SetVolume(double Volume) { this->Volume = Volume; }

Cart::Cart() : RestID(-1) {}

void Cart::AddItem(Item* item, int count, int id) {
    if (items.empty()) RestID = id;
    for (int i=0; i<items.size(); i++) {
        if (items[i].first->GetID() == item->GetID()) {
            items[i].second += count;
            return;
        }
    }
    items.push_back(make_pair(item, count));
}

void Cart::UpdateCount(int ind, int count) {
    items[ind].second += count;
}

void Cart::RemoveItem(int ind) {
    items.erase(items.begin() + ind);
    if (items.empty()) RestID = -1;
}

double Cart::GetPrice() {
    double tot = 0;
    for (int i=0; i<items.size(); i++) {
        tot += items[i].first->GetPrice() * items[i].second;
    }
    return tot;
}

void Cart::Clear() { 
    items.clear(); 
    RestID = -1;
}

int Cart::GetSize() {
    return items.size();
}

vector <pair<Item*, int>> Cart::GetItems() { return items; }

void Cart::Display() {
    if (items.size() == 0) {
        cout << "=== Yor Cart Is Empty! ===\n";
        return;
    }
    cout << "=== Your Cart ===";
    for (int i=0; i<items.size(); i++) {
        cout << "[ " << i+1 << " ] " << items[i].first->Getname()
             << "( Amount = " << items[i].second << " , Price = " << items[i].first->GetPrice() << " )\n";
    }
    cout << "Total Price : " << this->GetPrice() << endl;
    cout << "Restaurant ID : " << RestID << endl;
}


ostream& operator << (ostream& output, Type type) {
    switch (type) {
        case Food: output << "Food"; break;
        case Drink: output << "Drink"; break;
        case Other: output << "Other"; break;
    }
    return output;
}
ostream& operator << (ostream& output, Status status) {
    switch (status) {
        case Active: output << "Active"; break;
        case InActive: output << "InActive"; break;
    }
    return output;
}
ostream& operator << (ostream& output, Role role) {
    switch (role) {
        case Customer: output << "Customer"; break;
        case Manager: output << "Manager"; break;
        case Admin: output << "Admin"; break;
    }
    return output;
}