#include "DAO.h"
#include "Classes.h"
#include <iostream>
#include <sstream>
using namespace std;

MenuItemDAO::MenuItemDAO(DatabaseManager& database) : db(database) {}

bool MenuItemDAO::Insert(Item* item) {
    if (!item) return false;
    
    stringstream sql;
    string typeStr;
    switch(item->Gettype()) {
        case Food: typeStr = "food"; break;
        case Drink: typeStr = "drink"; break;
        default: typeStr = "other";
    }
    
    if (item->Gettype() == Food) {
        FoodC* food = dynamic_cast<FoodC*>(item);
        sql << "INSERT INTO menu_items (id, restaurant_id, name, description, price, type, is_available, prep_time) VALUES ("
            << item->GetID() << ", "
            << "1, '"  // restaurant_id - بعداً باید از کجا بیاید؟
            << item->Getname() << "', '"
            << item->Getdesc() << "', "
            << item->GetPrice() << ", '"
            << typeStr << "', "
            << (item->Getstatus() ? 1 : 0) << ", "
            << (food ? food->GetPrep() : 0) << ");";
    } 
    else if (item->Gettype() == Drink) {
        DrinkC* drink = dynamic_cast<DrinkC*>(item);
        sql << "INSERT INTO menu_items (id, restaurant_id, name, description, price, type, is_available, volume) VALUES ("
            << item->GetID() << ", "
            << "1, '"
            << item->Getname() << "', '"
            << item->Getdesc() << "', "
            << item->GetPrice() << ", '"
            << typeStr << "', "
            << (item->Getstatus() ? 1 : 0) << ", "
            << (drink ? drink->GetVolume() : 0) << ");";
    }
    else {
        sql << "INSERT INTO menu_items (id, restaurant_id, name, description, price, type, is_available) VALUES ("
            << item->GetID() << ", "
            << "1, '"
            << item->Getname() << "', '"
            << item->Getdesc() << "', "
            << item->GetPrice() << ", '"
            << typeStr << "', "
            << (item->Getstatus() ? 1 : 0) << ");";
    }
    
    return db.execute(sql.str());
}

bool MenuItemDAO::Update(Item* item) {
    if (!item) return false;
    stringstream sql;
    string typeStr;
    switch(item->Gettype()) {
        case Food: typeStr = "food"; break;
        case Drink: typeStr = "drink"; break;
        default: typeStr = "other";
    }
    
    sql << "UPDATE menu_items SET "
        << "name = '" << item->Getname() << "', "
        << "description = '" << item->Getdesc() << "', "
        << "price = " << item->GetPrice() << ", "
        << "type = '" << typeStr << "', "
        << "is_available = " << (item->Getstatus() ? 1 : 0) << " ";
    
        if (item->Gettype() == Food) {
        FoodC* food = dynamic_cast<FoodC*>(item);
        sql << ", prep_time = " << (food ? food->GetPrep() : 0) << " ";
    }
    else if (item->Gettype() == Drink) {
        DrinkC* drink = dynamic_cast<DrinkC*>(item);
        sql << ", volume = " << (drink ? drink->GetVolume() : 0) << " ";
    }
    
    sql << "WHERE id = " << item->GetID() << ";";
    
    return db.execute(sql.str());
}

bool MenuItemDAO::Remove(int id) {
    stringstream sql;
    sql << "DELETE FROM menu_items WHERE id = " << id << ";";
    return db.execute(sql.str());
}

int MenuItemDAO::ItemCallback(void* data, int argc, char** argv, char** azColName) {
    vector<Item*>* items = static_cast<vector<Item*>*>(data);
    
    int id = argv[0] ? atoi(argv[0]) : 0;
    int restaurantId = argv[1] ? atoi(argv[1]) : 0;
    string name = argv[2] ? argv[2] : "";
    string description = argv[3] ? argv[3] : "";
    double price = argv[4] ? atof(argv[4]) : 0.0;
    string typeStr = argv[5] ? argv[5] : "food";
    int isAvailable = argv[6] ? atoi(argv[6]) : 1;
    
    Type type;
    if (typeStr == "food") type = Food;
    else if (typeStr == "drink") type = Drink;
    else type = Other;
    
    Status status = (isAvailable == 1) ? Active : InActive;
    Item* item = nullptr;
    
    if (type == Food) {
        int prepTime = (argc > 7 && argv[7]) ? atoi(argv[7]) : 30;
        item = new FoodC(id, name, description, price, type, status, prepTime);
    }
    else if (type == Drink) {
        double volume = (argc > 7 && argv[7]) ? atof(argv[7]) : 250;
        item = new DrinkC(id, name, description, price, type, status, volume);
    }
    else {
        item = new Item(id, name, description, price, type, status);
    }
    items->push_back(item);
    return 0;
}

Item* MenuItemDAO::FindById(int id) {
    stringstream sql;
    sql << "SELECT * FROM menu_items WHERE id = " << id << ";";
    vector<Item*> results;
    if (db.query(sql.str(), ItemCallback, &results)) {
        if (!results.empty()) {
            return results[0];
        }
    }
    return nullptr;
}

vector<Item*> MenuItemDAO::FindByRestaurant(int restaurantId) {
    vector<Item*> results;
    stringstream sql;
    sql << "SELECT * FROM menu_items WHERE restaurant_id = " << restaurantId << ";";
    db.query(sql.str(), ItemCallback, &results);
    return results;
}