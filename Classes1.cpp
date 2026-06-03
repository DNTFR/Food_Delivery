#include "Classes.h"
#include <iostream>
using namespace std;

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
    cout << "---Item Details---\n";
    cout << "\t" << type << "[" << status << "]\n";
    cout << "\t" << name << "(" << ID << ")\n";
    cout << "\tDescription: " << etc << endl;
    cout << "\tPrice: " << Price << endl;
    cout << "------------------\n";
}

Menu::Menu() : RestaurantID(0) {}

Menu::Menu(int ID) : RestaurantID(ID) {}

int Menu::GetRestaurantId() { return RestaurantID; }

vector <Item*> Menu::GetItems() {
    for (int i=0; i<MenuItems.size(); i++) {
        MenuItems[i]->Display();
    }
}

int Menu::FindItemIndex(int id) {
    for (int i=0; i<MenuItems.size(); i++) {
        if (MenuItems[i]->GetID() == id) return i;
    }
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
